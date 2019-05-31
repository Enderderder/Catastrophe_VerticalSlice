// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "GuardAiController.h"
#include "GuardAnimInstance.h"

#include "RespawnSystem/RespawnSubsystem.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NavCapsule"));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_No;

	HeadHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HeadHitBox"));
	HeadHitbox->SetGenerateOverlapEvents(true);
	HeadHitbox->SetCollisionProfileName(TEXT("EnemyBlock"));
	HeadHitbox->ComponentTags.Add(TEXT("Head"));
	HeadHitbox->CanCharacterStepUpOn = ECB_No;
	HeadHitbox->SetupAttachment(GetMesh(), TEXT("HeadSocket"));

	BodyHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyHitBox"));
	BodyHitBox->SetGenerateOverlapEvents(true);
	BodyHitBox->SetCollisionProfileName(TEXT("EnemyBlock"));
	BodyHitBox->ComponentTags.Add(TEXT("Body"));
	BodyHitBox->CanCharacterStepUpOn = ECB_No;
	BodyHitBox->SetupAttachment(GetMesh());

	CatchHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CatchHitBox"));
	CatchHitBox->SetGenerateOverlapEvents(true);
	CatchHitBox->SetCollisionProfileName(TEXT("EnemyOverlap"));
	CatchHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // No collision to begin with
	CatchHitBox->CanCharacterStepUpOn = ECB_No;
	CatchHitBox->OnComponentBeginOverlap.AddDynamic(this, &AGuard::OnCatchHitBoxOverlap);
	CatchHitBox->SetupAttachment(GetMesh());

	// TODO: Pls remove this
	StupidFakeBsHearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("StupidFakeBsHearingSphere"));
	StupidFakeBsHearingSphere->SetGenerateOverlapEvents(true);
	StupidFakeBsHearingSphere->SetCollisionProfileName(TEXT("EnemyOverlap"));
	StupidFakeBsHearingSphere->CanCharacterStepUpOn = ECB_No;
	StupidFakeBsHearingSphere->SetupAttachment(GetMesh());

	AlertMarkMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AlertMarkMesh"));
	AlertMarkMesh->SetGenerateOverlapEvents(false);
	AlertMarkMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AlertMarkMesh->SetupAttachment(GetMesh());

	QuestionMarkMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("QuestionMarkMesh"));
	QuestionMarkMesh->SetGenerateOverlapEvents(false);
	QuestionMarkMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	QuestionMarkMesh->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();
	
	// Sets the anim instance
	GuardAnimInstance = Cast<UGuardAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGuard::GetPerceptionLocRot_Implementation(FVector& Location, FRotator& Rotation) const
{
	Location = GetActorLocation() + FVector(0.0f, 0.0f, 80.0f);
	Rotation = GetActorRotation();
}

void AGuard::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	/// Not using parent class implementation

	GetPerceptionLocRot(Location, Rotation);
}

void AGuard::OnSightPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus)
{
	
}

void AGuard::OnHearingPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus)
{

}

void AGuard::SetGuardState(EGuardState _newState)
{
	EGuardState oldState = GuardState;
	GuardState = _newState;

	OnGuardStateChange(oldState, _newState);
}

void AGuard::OnGuardStateChange_Implementation(EGuardState _oldState, EGuardState _newState)
{
	// Refresh the animations of the guard
	StopAllMontages();

	// What will happened when exiting each state
	switch (_oldState)
	{
	case EGuardState::STATIONARY:
		break;
	case EGuardState::SLEEPING:
		break;
	case EGuardState::WAKEUPSTATEONE:
		break;
	case EGuardState::WAKEUPSTATETWO:
		break;
	case EGuardState::PATROLLING:
		break;
	case EGuardState::INVESTATING:
		ToggleQuestionIndicator(false);
		break;
	case EGuardState::CHASING:
		ToggleAlertIndicator(false);
		CatchHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EGuardState::SEARCHING:
		ToggleQuestionIndicator(false);
		bPlayerWasInSight = false;
		break;
	case EGuardState::STUNED:
		break;
	default:
		break;
	}

	// What will happened when entering each state
	switch (_newState)
	{
	case EGuardState::STATIONARY:
		GuardController->ModifySightRange(PatrolSightRange, LosingSightRange);
		break;

	case EGuardState::SLEEPING:
		GuardController->ModifySightRange(0.0f, LosingSightRange);
		break;

	case EGuardState::WAKEUPSTATEONE:
		break;

	case EGuardState::WAKEUPSTATETWO:
		break;

	case EGuardState::PATROLLING:
		if (bPatrolBehaviour && PatrolLocations.Num() > 0)
		{
			SetGuardMaxSpeed(PatrolSpeed);
			GuardController->ModifySightRange(PatrolSightRange, LosingSightRange);
		}
		else SetGuardState(DefaultGuardState); // If guard dont have a patrol behaviour, go back to the default state
		break;

	case EGuardState::INVESTATING:
		SetGuardMaxSpeed(PatrolSpeed);
		GuardController->ModifySightRange(PatrolSightRange, LosingSightRange);
		ToggleQuestionIndicator(true);
		break;

	case EGuardState::CHASING:
		SetGuardMaxSpeed(ChaseSpeed);
		GuardController->ModifySightRange(ChasingSightRange, LosingSightRange);
		ToggleAlertIndicator(true);
		CatchHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;

	case EGuardState::SEARCHING:
		SetGuardMaxSpeed(ChaseSpeed);
		break;

	case EGuardState::STUNED:
		OnStunBegin();
		break;


	default: break;
	}
}

void AGuard::OnStunBegin()
{
	// Make sure this bi** doesnt move
	GuardController->StopMovement();

	// Sight goes dark for guard
	GuardController->ModifySightRange(0.0f);
	if (ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		GuardController->GetBlackboardComponent()->SetValueAsVector(
			TEXT("PlayerLastSeenLocation"), player->GetActorLocation());
	}

	if (GuardAnimInstance)
		GuardAnimInstance->bStuned = true;
	else
	{
		// Force the animation
		Cast<UGuardAnimInstance>(GetMesh()->GetAnimInstance())->bStuned = true;
	}
	// Clear the old timer
	GetWorld()->GetTimerManager().ClearTimer(StunTimerHnadle);
	// Set a timer to call OnStunEnd()
	GetWorld()->GetTimerManager().SetTimer(
		StunTimerHnadle, this, &AGuard::OnStunEnd, MaxStunTime, false);

	// Call the blueprint implementation
	Receive_OnStunBegin();
}

void AGuard::OnStunEnd()
{
	GuardAnimInstance->bStuned = false;

	// Go search the player if he saw the player
	if (bPlayerWasInSight)
	{
		SetGuardState(EGuardState::SEARCHING);
	}
	else // Or just go back to the default state
	{
		SetGuardState(DefaultGuardState);
	}

	// Call the blueprint implementation
	Receive_OnStunEnd();
}

void AGuard::OnCatchHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		//FLoadStreamingLevelInfo levelStreamInfo;
		//levelStreamInfo.
		//levelStreamInfo.bUnloadCurrentLevel = false;
		//levelStreamInfo.bBlockOnLoad = false;

		//URespawnSubsystem::GetInst(this)->LoadLevelStreaming()
	}
}

void AGuard::SetGuardMaxSpeed(float _speed)
{
	GetCharacterMovement()->MaxWalkSpeed = _speed;
}

void AGuard::StopAllMontages()
{
	if (GuardAnimInstance)
		GuardAnimInstance->StopAllMontages(0.1f);
}

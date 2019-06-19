// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "GuardAiController.h"
#include "GuardAnimInstance.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

#include "RespawnSystem/RespawnSubsystem.h"

#include "Engine.h"

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

	ZzzMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ZzzMesh"));
	ZzzMesh->SetGenerateOverlapEvents(false);
	ZzzMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZzzMesh->SetupAttachment(GetMesh());

	HeadLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("HeadLight"));
	HeadLight->SetRelativeLocation(FVector(16.0f, 0.0f, 0.0f));
	HeadLight->InnerConeAngle = 10.0f;
	HeadLight->OuterConeAngle = 30.0f;
	HeadLight->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	// Sets default values
	DefaultTransform = GetActorTransform();
	
	// Sets the anim instance
	GuardAnimInstance = Cast<UGuardAnimInstance>(GetMesh()->GetAnimInstance());
	if (!GuardAnimInstance)
		UE_LOG(LogTemp, Error, TEXT("Failed to initiate guard anim instance"));

	// Gets reference from the player
	PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!PlayerRef)
		UE_LOG(LogTemp, Error, TEXT("Failed to get player reference"));
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPlayerInSleepDetectRange)
	{
		switch (GuardState)
		{
		case EGuardState::SLEEPING:
		{
			if (PlayerRef && bPlayerInSleepDetectRange)
			{
				if (PlayerRef->GetVelocity().Size() >= 50.0f
					&& !PlayerRef->GetCharacterMovement()->IsCrouching())
				{
					SetGuardState(EGuardState::WAKEUP_STAGEONE);
					float time;
					LookAround(time);
				}
			}

			break;
		}
		case EGuardState::WAKEUP_STAGEONE:
			break;

		case EGuardState::WAKEUP_STAGETWO:
			break;

		default: break;
		}
	}

	if (GuardState == EGuardState::CHASING)
	{

	}
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
		ToggleZzzIndicator(false);
		HeadLight->SetVisibility(true);
		if (GuardAnimInstance)
			GuardAnimInstance->bSleeping = false;
		break;
	case EGuardState::WAKEUP_STAGEONE:
		break;
	case EGuardState::WAKEUP_STAGETWO:
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
		ToggleZzzIndicator(true);
		HeadLight->SetVisibility(false);
		if (GuardAnimInstance)
			GuardAnimInstance->bSleeping = true;
		break;

	case EGuardState::WAKEUP_STAGEONE:
		GuardController->ModifySightRange(PatrolSightRange, LosingSightRange);
		break;

	case EGuardState::WAKEUP_STAGETWO:
		break;

	case EGuardState::PATROLLING:
		HeadLight->SetLightColor(NormalHeadLightColor);
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
		HeadLight->SetLightColor(NormalHeadLightColor);
		break;

	case EGuardState::CHASING:
		SetGuardMaxSpeed(ChaseSpeed);
		GuardController->ModifySightRange(ChasingSightRange, LosingSightRange);
		ToggleAlertIndicator(true);
		HeadLight->SetLightColor(SpottedHeadLightColor);
		CatchHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;

	case EGuardState::SEARCHING:
		SetGuardMaxSpeed(ChaseSpeed);
		HeadLight->SetLightColor(AlertedHeadLightColor);
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

	// Turn off head light
	HeadLight->SetVisibility(false);

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

	// Turn the head light back on
	HeadLight->SetVisibility(true);

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
	// If player get caught
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			OnCatchPlayer(player);
		}
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

void AGuard::LookAround_Implementation(float& out_montageTime)
{
	/// Blueprint should do the thing
}

void AGuard::OnCatchPlayer_Implementation(APlayerCharacter* _player)
{
	UE_LOG(LogTemp, Warning, TEXT("Player caught"));

	/// Should be implement in derived class
}

void AGuard::ResetGuard()
{
	GetCharacterMovement()->DisableMovement();
	GuardController->ModifySightRange(0.0f);
	GuardController->StopMovement();
	bPlayerWasInSight = false;
	bPlayerInSight = false;
	SetGuardState(DefaultGuardState);
	StopAllMontages();
	//SetActorTransform(DefaultTransform, false, nullptr, ETeleportType::ResetPhysics
	SetActorLocationAndRotation(DefaultTransform.GetLocation(), DefaultTransform.GetRotation());

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

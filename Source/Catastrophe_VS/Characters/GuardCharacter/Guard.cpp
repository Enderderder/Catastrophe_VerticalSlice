// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "GuardAiController.h"
#include "GuardAnimInstance.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NavCapsule"));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_No;

	HeadHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HeadHitBox"));
	HeadHitbox->SetCollisionProfileName(TEXT("Enemy"));
	HeadHitbox->ComponentTags.Add(TEXT("Head"));
	HeadHitbox->CanCharacterStepUpOn = ECB_No;
	HeadHitbox->SetupAttachment(GetMesh(), TEXT("HeadSocket"));

	BodyHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyHitBox"));
	BodyHitBox->SetCollisionProfileName(TEXT("Enemy"));
	HeadHitbox->ComponentTags.Add(TEXT("Body"));
	BodyHitBox->CanCharacterStepUpOn = ECB_No;
	BodyHitBox->SetupAttachment(GetMesh());

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
	// Sight goes dark for guard
	GuardController->ModifySightRange(0.0f);


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

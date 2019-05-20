// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

#include "GuardAiController.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	HeadHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HeadHitBox"));
	HeadHitbox->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();
	
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
	switch (_newState)
	{
	case EGuardState::STATIONARY:
		break;

	case EGuardState::SLEEPING:
		GuardControllerRef->GetSightDefaultConfig()->SightRadius = 0.0f;
		break;

	case EGuardState::WAKEUPSTATEONE:
		break;

	case EGuardState::WAKEUPSTATETWO:
		break;

	case EGuardState::PATROLLING:
		if (bPatrolBehaviour && PatrolLocations.Num() > 0)
		{
			SetGuardMaxSpeed(PatrolSpeed);
			GuardControllerRef->ModifySightRange(0.0f);
		}
		else SetGuardState(_oldState); // If guard dont have a patrol behaviour, dont do it
		break;

	case EGuardState::INVESTATING:
		break;

	case EGuardState::CHASING:
		SetGuardMaxSpeed(ChaseSpeed);
		break;

	case EGuardState::SEARCHING:

		break;

	case EGuardState::STUNED:
		break;


	default: break;
	}
}

void AGuard::SetGuardMaxSpeed(float _speed)
{
	GetCharacterMovement()->MaxWalkSpeed = _speed;
}

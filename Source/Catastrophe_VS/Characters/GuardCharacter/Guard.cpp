// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;




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

void AGuard::SetGuardMaxSpeed(float _speed)
{
	GetCharacterMovement()->MaxWalkSpeed = _speed;
}

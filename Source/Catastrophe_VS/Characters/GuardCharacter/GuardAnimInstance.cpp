// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Guard.h"

void UGuardAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	// Gets the reference to the guard character
	OwnerGuardActor = Cast<AGuard>(TryGetPawnOwner());

}

void UGuardAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (OwnerGuardActor && !OwnerGuardActor->IsPendingKill())
	{
		// Gets the speed of the character
		MoveSpeed = OwnerGuardActor->GetCharacterMovement()->Velocity.Size();
	}
}
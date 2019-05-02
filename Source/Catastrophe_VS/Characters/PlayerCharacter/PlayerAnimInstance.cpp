// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "PlayerCharacter.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	// Store player character reference
	CustomPlayerCharacter = Cast<APlayerCharacter>(GetOwningActor());
	if (!CustomPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("CustomPlayerCharacter is null, check player character class"));
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//TODO Check
	// Update animation data
	if (CustomPlayerCharacter->IsValidLowLevel())
	{
		if (UCharacterMovementComponent* PlayerMovementComponent 
			= CustomPlayerCharacter->GetCharacterMovement())
		{
			Speed = PlayerMovementComponent->Velocity.Size();
			// No Anim Blend Space movement calculation
			bRunning = (Speed != 0.0f);

			bInAir = PlayerMovementComponent->IsFalling();
			bCrouch = PlayerMovementComponent->IsCrouching();
		}
	}
}

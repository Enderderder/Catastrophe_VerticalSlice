// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"
#include "SimpleInteractableAnimated.h"
#include "SimpleInteractableStatic.h"
#include "Components/BoxComponent.h"

// Add default functionality here for any IInteractableObject functions that are not pure virtual.

void IInteractableObject::OnInteract_Implementation(class APlayerCharacter* _actor)
{
	/// Interface Template function should not have any implementation
}

void IInteractableObject::OnUnInteract_Implementation(class APlayerCharacter* _actor)
{
	/// Interface Template function should not have any implementation
}

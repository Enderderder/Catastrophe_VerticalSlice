// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "Components/PrimitiveComponent.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// This component will not tick
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::OnTriggerWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// See if we already holds the player character pointer
	if (PlayerRef && !PlayerRef->IsPendingKill())
	{
		if (PlayerRef == OtherActor)
		{
			PlayerRef->SetInteractionTarget(GetOwner());
			TriggerCounter++;
		}
	}
	else if (OtherActor->IsA<APlayerCharacter>())
	{
		PlayerRef = Cast<APlayerCharacter>(OtherActor);
		PlayerRef->SetInteractionTarget(GetOwner());
		TriggerCounter++;
	}

	// If this component has set to auto interact, interact immediatly
	if (bAutoInteract && PlayerRef && !PlayerRef->IsPendingKill())
	{
		Interact(PlayerRef, 0.0f);
	}
}

void UInteractableComponent::OnTriggerEndWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerRef == OtherActor)
	{
		TriggerCounter--;
		if (TriggerCounter <= 0)
		{
			PlayerRef->ResetInteractionAction();
			PlayerRef->RemoveInteractionTarget(GetOwner());
		}
	}
}

void UInteractableComponent::Interact(class APlayerCharacter* _playerCharacter, float _holdTime)
{
	HoldingTime = _holdTime;

	if (bCanInteract && _holdTime >= RequiredHoldTime)
	{
		// Calls the interact functions
		OnInteract.Broadcast(_playerCharacter);

		// Reset the holding time
		HoldingTime = 0.0f;
		_playerCharacter->ResetInteractionAction();

		// If the component has set to one time use, disable after interaction
		if (bOneTimeUse)
			bCanInteract = false;
	}
}

void UInteractableComponent::InteractHold(class APlayerCharacter* _playerCharacter, float _holdTime)
{
	if (bCanInteract)
	{
		
	}
}

void UInteractableComponent::RegisterTriggerVolume(class UPrimitiveComponent* _component)
{
	// Force the component to generate overlap events
	_component->SetGenerateOverlapEvents(true);

	// Binds the function to the component event
	_component->OnComponentBeginOverlap.AddDynamic(
		this, &UInteractableComponent::OnTriggerWithPlayer);
	_component->OnComponentEndOverlap.AddDynamic(
		this, &UInteractableComponent::OnTriggerEndWithPlayer);
}


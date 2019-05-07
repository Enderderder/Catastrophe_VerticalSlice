// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "Components/PrimitiveComponent.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	
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
	else if (APlayerCharacter* playerRef = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerRef = playerRef;
		PlayerRef->SetInteractionTarget(GetOwner());
		TriggerCounter++;
	}
}

void UInteractableComponent::OnTriggerEndWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerRef == OtherActor)
	{
		TriggerCounter--;
		if (TriggerCounter <= 0)
		{
			PlayerRef->RemoveInteractionTarget(GetOwner());
		}
	}
}

// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableComponent::Interact(class APlayerCharacter* _playerCharacter)
{
	OnInteract.Broadcast(_playerCharacter);
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


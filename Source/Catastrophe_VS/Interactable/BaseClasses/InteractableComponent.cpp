// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// This component will not tick
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractableComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Checks if we should show the interactable UI
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (playerPawn && InteractableUI)
	{
		// Distance check
		float distanceToPlayer;
		distanceToPlayer = FVector::Dist(
			InteractableUI->GetComponentLocation(), playerPawn->GetActorLocation());
		if (distanceToPlayer <= UIShowingDistance
			&& bCanInteract)
		{
			bWantToShowUI = true;
		}
		else
		{
			bWantToShowUI = false;
		}

		// Toggle the UI if needed
		if (!bShowingUI && bWantToShowUI) // Not showing but need to show
		{
			bShowingUI = true;
			InteractableUI->SetVisibility(true);
		}
		else if (bShowingUI && !bWantToShowUI) // Showing but need to hide
		{
			bShowingUI = false;
			InteractableUI->SetVisibility(false);
		}

		// Rotate the ui towards camera when it is showing
		if (bShowingUI)
		{
			FVector cameraLoc = UGameplayStatics::GetPlayerCameraManager(
				this, 0)->GetCameraLocation();
			FRotator uiRot = (cameraLoc - InteractableUI->GetComponentLocation()).Rotation();
			InteractableUI->SetWorldRotation(uiRot);
		}
	}
}

void UInteractableComponent::OnTriggerWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!PlayerRef && OtherActor->IsA<APlayerCharacter>())
	{
		PlayerRef = Cast<APlayerCharacter>(OtherActor);
	}

	// See if we already holds the player character pointer
	if (PlayerRef && !PlayerRef->IsPendingKill())
	{
		if (PlayerRef == OtherActor)
		{
			if (bCanInteract)
			{
				PlayerRef->SetInteractionTarget(this);
				//PlayerRef->ToggleInteractUI(true); // Enable later for testing player ui
			}
			TriggerCounter++;
		}
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
			PlayerRef->RemoveInteractionTarget(this);
			//PlayerRef->ToggleInteractUI(false);
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

void UInteractableComponent::RegisterUiComponent(class USceneComponent* _uiComponent)
{
	if (_uiComponent)
	{
		InteractableUI = _uiComponent;
		InteractableUI->SetVisibility(false);
	}
}

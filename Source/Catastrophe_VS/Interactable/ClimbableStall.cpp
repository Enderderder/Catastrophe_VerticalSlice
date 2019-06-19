// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbableStall.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interactable/BaseClasses/InteractableComponent.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"

AClimbableStall::AClimbableStall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(false);
	RootComponent = Mesh;
	//Mesh->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(RootComponent);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));

	// Set the default state
	WayPointCount = 0;
	bHasUsed = false;
	bInUse = false;
}

void AClimbableStall::BeginPlay()
{
	Super::BeginPlay();

	InteractableComponent->RegisterTriggerVolume(TriggerBox);
	InteractableComponent->OnInteract.AddDynamic(this, &AClimbableStall::InteractionStarting);
}

float AClimbableStall::FindTotalDistance()
{
	float totalDistanceResult = 0.0f;

	for (int i = 0; i < WayPointArray.Num(); ++i)
	{
		if (i == 0)
		{
			totalDistanceResult +=
				FVector::Distance(PlayerReference->GetActorLocation(), WayPointArray[0]->GetComponentLocation());
			continue;
		}

		totalDistanceResult +=
			FVector::Distance(WayPointArray[i - 1]->GetComponentLocation(), WayPointArray[i]->GetComponentLocation());
	}

	return totalDistanceResult;
}

void AClimbableStall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Run the check for the interaction
	if (bHasSetPath && bInUse && !bHasUsed)
	{
		bool reachedDestination = false;

		// Increament the time the move
		LerpDelta += DeltaTime;
		float alpha = LerpDelta / TimeForCurrentPath;

		float leakTime = 0.0f;
		if (alpha > 1.0f)
		{
			float leakAlpha = alpha - 1.0f;
			leakTime = leakAlpha / TimeForCurrentPath;
			alpha = 1.0f;
			reachedDestination = true;
		}

		// Move the actor
		FVector resultLocation;
		resultLocation = FMath::Lerp(CurrentWayPointLoc, DestinationLoc, alpha);
		PlayerReference->SetActorLocation(resultLocation);

		// Check if the player has reach the current destination
		if (reachedDestination)
		{
			WayPointCount++;
			if (WayPointCount >= WayPointArray.Num())
			{
				// End the interaction and unlock player movement
				InteractionEnd();
			}
			else
			{
				LerpDelta = leakTime; // Reset the lerp delta time and add the leak time onto it
				CurrentWayPointLoc = DestinationLoc;
				DestinationLoc = WayPointArray[WayPointCount]->GetComponentLocation();
				CurrentPathLength = FVector::Distance(CurrentWayPointLoc, DestinationLoc);
				TimeForCurrentPath = (CurrentPathLength / TotalDistanceTravel) * TotalTransferTime;
			}
		}
	}
}

void AClimbableStall::InteractionStarting(class APlayerCharacter* _playerCharacter)
{
	PlayerReference = _playerCharacter;
	if (bUsable)
	{
		Recieve_InteractionStart();

		// Set the first way point for the action
		CurrentWayPointLoc = PlayerReference->GetActorLocation();
		DestinationLoc = WayPointArray[0]->GetComponentLocation();
		TotalDistanceTravel = FindTotalDistance();
		CurrentPathLength = FVector::Distance(CurrentWayPointLoc, DestinationLoc);
		TimeForCurrentPath = (CurrentPathLength / TotalDistanceTravel) * TotalTransferTime;

		// Initiate the interaction
		bHasSetPath = true;
		bInUse = true;
		LerpDelta = 0.0f;

		// Disable the player action
		PlayerReference->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		PlayerReference->RemoveInteractionTarget(InteractableComponent);
	}
}

void AClimbableStall::InteractionEnd()
{
	Recieve_InteractionEnd();

	// Re-Enable the player action
	PlayerReference->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Disable the interaction ability for this interactable actor
	bUsable = false;
	bHasUsed = true;
	bInUse = false;

	// Disable the tick function for this actor
	//DisableInteractActor();
}
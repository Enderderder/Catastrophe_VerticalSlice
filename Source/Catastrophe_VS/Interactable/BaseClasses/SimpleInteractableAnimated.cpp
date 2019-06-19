// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleInteractableAnimated.h"

#include "Components/BoxComponent.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"

// Sets default values
ASimpleInteractableAnimated::ASimpleInteractableAnimated()
{
	// This is not a tick actor
	PrimaryActorTick.bCanEverTick = false;

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	InteractionTrigger->SetupAttachment(RootComponent);
	// Bind the func to the component
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASimpleInteractableAnimated::OnInteractionTriggerOverlapBegin);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &ASimpleInteractableAnimated::OnInteractionTriggerOverlapEnd);
}

void ASimpleInteractableAnimated::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASimpleInteractableAnimated::OnInteractionTriggerOverlapBegin(class UPrimitiveComponent* _overlappedComponent, class AActor* _otherActor, class UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (auto player = Cast<APlayerCharacter>(_otherActor)) 
	{
		//player->SetInteractionTarget(InteractableComponent);
	}
}

void ASimpleInteractableAnimated::OnInteractionTriggerOverlapEnd(class UPrimitiveComponent* _overlappedComponent, class AActor* _otherActor, class UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
	if (auto player = Cast<APlayerCharacter>(_otherActor))
	{
		//player->RemoveInteractionTarget(this);
	}
}
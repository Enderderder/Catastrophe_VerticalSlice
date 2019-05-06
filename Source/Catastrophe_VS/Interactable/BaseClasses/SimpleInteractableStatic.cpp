// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleInteractableStatic.h"

#include "Components/BoxComponent.h"

// Set default value
ASimpleInteractableStatic::ASimpleInteractableStatic()
{
	// This is not a tick actor
	/*PrimaryActorTick.bCanEverTick = false;

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	InteractionTrigger->SetupAttachment(RootComponent);*/
}

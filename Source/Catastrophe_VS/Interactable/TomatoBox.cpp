// Fill out your copyright notice in the Description page of Project Settings.

#include "TomatoBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Engine/CollisionProfile.h"

#include "Interactable/BaseClasses/InteractableComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

ATomatoBox::ATomatoBox()
{
	PrimaryActorTick.bCanEverTick = false;

	TomatoBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TomatoBoxMesh"));
	TomatoBoxMesh->SetGenerateOverlapEvents(false);
	TomatoBoxMesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	RootComponent = TomatoBoxMesh;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetGenerateOverlapEvents(true);
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	TriggerVolume->SetupAttachment(RootComponent);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->bOneTimeUse = false;
	InteractableComponent->RegisterTriggerVolume(TriggerVolume);
	InteractableComponent->OnInteract.AddDynamic(this, &ATomatoBox::PickUpTomato);

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));

}

void ATomatoBox::BeginPlay()
{
	Super::BeginPlay();

}

void ATomatoBox::PickUpTomato(APlayerCharacter* _playerCharacter)
{
	Receive_PickUpTomato();

	// Restore the tomato accordingly
	if (bRestoreAllTomatoOneTime)
	{
		_playerCharacter->RestoreAllTomatos();
	}
	else
	{
		_playerCharacter->RestoreTomato(1);
	}
}

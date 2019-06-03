// Fill out your copyright notice in the Description page of Project Settings.

#include "StackedCrates.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Interactable/BaseClasses/InteractableComponent.h"

// Set default values
AStackedCrates::AStackedCrates()
{
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	CratesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CratesMesh"));
	CratesMesh->SetupAttachment(RootComponent);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetGenerateOverlapEvents(true);
	TriggerVolume->SetupAttachment(CratesMesh);

	BlockVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockVolume"));
	BlockVolume->bAutoActivate = false;
	BlockVolume->SetCanEverAffectNavigation(true);
	BlockVolume->SetGenerateOverlapEvents(false);
	//BlockVolume->SetCollisionProfileName(TEXT("Obstacle"));
	BlockVolume->SetupAttachment(CratesMesh);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->RegisterTriggerVolume(TriggerVolume);
	InteractableComponent->OnInteract.AddDynamic(this, &AStackedCrates::OnPlayerInteract);
}

void AStackedCrates::BeginPlay()
{
	Super::BeginPlay();

}

void AStackedCrates::OnPlayerInteract(class APlayerCharacter* _playerCharacter)
{
	Receive_OnPlayerInteract();

	// This is a one time use
	InteractableComponent->bCanInteract = false;

	// Plays the animation
	Receive_PlayCrateAnim();

	// Set collision boxes
	CratesMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BlockVolume->Activate(true);
}

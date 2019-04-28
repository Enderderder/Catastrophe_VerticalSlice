// Fill out your copyright notice in the Description page of Project Settings.

#include "StackedCrates.h"

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Set default values
AStackedCrates::AStackedCrates()
{
	BlockVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockVolume"));
	BlockVolume->bAutoActivate = false;
	BlockVolume->SetCanEverAffectNavigation(true);
	BlockVolume->SetGenerateOverlapEvents(false);
	//BlockVolume->SetCollisionProfileName(TEXT("Obstacle"));
	BlockVolume->SetupAttachment(RootComponent);
}

void AStackedCrates::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStackedCrates::OnInteract_Implementation(AActor* _actor)
{
	Super::OnInteract_Implementation(_actor);

	Receive_PlayCrateAnim();

	GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BlockVolume->Activate(true);
}

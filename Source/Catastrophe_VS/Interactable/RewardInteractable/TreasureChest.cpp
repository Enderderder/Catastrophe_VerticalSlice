// Fill out your copyright notice in the Description page of Project Settings.

#include "TreasureChest.h"

#include "Components/StaticMeshComponent.h"
//#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Interactable/BaseClasses/InteractableComponent.h"

// Sets default values
ATreasureChest::ATreasureChest()
{
 	// This is not a tick actor
	PrimaryActorTick.bCanEverTick = false;

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	//ChestMesh->SetCollisionProfileName(TEXT("PureStaticMesh"));
	RootComponent = ChestMesh;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	//TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(RootComponent);

	InteractableCompoenent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableCompoenent"));
	InteractableCompoenent->RegisterTriggerVolume(TriggerBox);
	InteractableCompoenent->OnInteract.AddDynamic(this, &ATreasureChest::OnPlayerInteract);
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATreasureChest::OnPlayerInteract(class APlayerCharacter* _playerCharacter)
{

}

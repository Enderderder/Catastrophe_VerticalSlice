// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Components/BoxComponent.h"
#include "Interactable/BaseClasses/InteractableComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(root);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	InteractableComponent->RegisterTriggerVolume(TriggerBox);
	InteractableComponent->OnInteract.AddDynamic(this, &ANPC::Interact);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Interact(class APlayerCharacter* _playerCharacter)
{
	Receive_Interact();
}


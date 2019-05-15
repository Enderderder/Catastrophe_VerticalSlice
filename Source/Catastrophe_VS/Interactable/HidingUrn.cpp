// Fill out your copyright notice in the Description page of Project Settings.

#include "HidingUrn.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

#include "Interactable/BaseClasses/InteractableComponent.h"

AHidingUrn::AHidingUrn()
{
	PrimaryActorTick.bCanEverTick = false;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(root);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->RegisterTriggerVolume(TriggerBox);
	InteractableComponent->OnInteract.AddDynamic(this, &AHidingUrn::OnPlayerInteract);
}

void AHidingUrn::BeginPlay()
{
	Super::BeginPlay();

	

}

void AHidingUrn::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
}

void AHidingUrn::HideInUrn()
{
	if (PlayerReference && !PlayerReference->IsPendingKill())
	{
		if (!bHasUsed)
		{
			if (bPlayerIn)
			{
				JumpOut();
			}
			else
			{
				JumpIn();
			}
		}
	}
}

void AHidingUrn::OnPlayerInteract(class APlayerCharacter* _playerCharacter)
{
	PlayerReference = _playerCharacter;
	HideInUrn();
}

//void AHidingUrn::OnInteract_Implementation(class APlayerCharacter* _actor)
//{
//	Super::OnInteract_Implementation(_actor);
//
//	HideInUrn();
//}

void AHidingUrn::JumpIn()
{
	Receive_JumpIn();
}

void AHidingUrn::JumpOut()
{
	Receive_JumpOut();
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "HidingUrn.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

AHidingUrn::AHidingUrn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHidingUrn::BeginPlay()
{
	Super::BeginPlay();

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

void AHidingUrn::OnInteract_Implementation(class APlayerCharacter* _actor)
{
	Super::OnInteract_Implementation(_actor);

	HideInUrn();
}

void AHidingUrn::JumpIn()
{
	Receive_JumpIn();
}

void AHidingUrn::JumpOut()
{
	Receive_JumpOut();
}
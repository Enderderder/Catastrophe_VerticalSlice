// Fill out your copyright notice in the Description page of Project Settings.

#include "TomatoBox.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

ATomatoBox::ATomatoBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATomatoBox::BeginPlay()
{
	Super::BeginPlay();

}

void ATomatoBox::PickUpTomato()
{
	if (PlayerReference && !PlayerReference->IsPendingKill())
	{
		// Give player 1 Tomato to his hand
		PlayerReference->RestoreTomato(1);
	}
}

void ATomatoBox::OnInteract_Implementation(class APlayerCharacter* _actor)
{
	Super::OnInteract_Implementation(_actor);

 	PickUpTomato();
	
	// End the interaction
	//Super::DisableStaticInteractable_Implementation(this);
}

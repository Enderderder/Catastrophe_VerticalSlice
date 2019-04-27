// Fill out your copyright notice in the Description page of Project Settings.

#include "TomatoBox.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"

ATomatoBox::ATomatoBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetupAttachment(RootComponent);
}

void ATomatoBox::BeginPlay()
{
	Super::BeginPlay();

}

void ATomatoBox::Tick(float DeltaTime)
{
	/// To enable this, make sure set PrimaryActorTick.bCanEverTick to true in constructor
	Super::Tick(DeltaTime);
}

void ATomatoBox::OnInteractionStart()
{
	Super::OnInteractionStart();

	// Safty check if the player reference is valid or not
	if (!PlayerReference)
	{
		PlayerReference = 
			Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	// Give player 1 Tomato to his hand
	PlayerReference->RestoreTomato(1);

	// End the interaction
	//OnInteractionEnd();
}

void ATomatoBox::OnInteractionEnd()
{
	Super::OnInteractionEnd();

	// Not able to use after
	//DisableInteractActor();

}

// Fill out your copyright notice in the Description page of Project Settings.

#include "HidingUrn.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DestructibleComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "Interactable/BaseClasses/InteractableComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

AHidingUrn::AHidingUrn()
{
	// Needed this for rotating the interaction text
	PrimaryActorTick.bCanEverTick = true;

	UrnDestructableMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("UrnDestructableMesh"));
	UrnDestructableMesh->CastShadow = false;
	UrnDestructableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UrnDestructableMesh->SetGenerateOverlapEvents(false);
	RootComponent = UrnDestructableMesh;

	UrnOutline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UrnOutline"));
	UrnOutline->SetGenerateOverlapEvents(false);
	UrnOutline->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(RootComponent);

	BlockVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockVolume"));
	BlockVolume->SetupAttachment(RootComponent);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->RegisterTriggerVolume(TriggerBox);
	InteractableComponent->OnInteract.AddDynamic(this, &AHidingUrn::OnPlayerInteract);
}

void AHidingUrn::BeginPlay()
{
	Super::BeginPlay();


}

void AHidingUrn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	// Clear all the timer handle
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AHidingUrn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Rotate the interaction text

}

void AHidingUrn::OnPlayerInteract(class APlayerCharacter* _playerCharacter)
{
	if (!bPlayerIn && bCanInteract)
	{
		JumpIn(_playerCharacter);
	}
	else if (bAllowManualJumpOut)
	{
		JumpOut(_playerCharacter);
	}
}

void AHidingUrn::JumpIn(class APlayerCharacter* _playerCharacter)
{
	// Notify that the player is in
	bPlayerIn = true;

	// Gives a timer that force player to jump out
	JumpOutTimerDel.BindUFunction(this, FName("JumpOut"), _playerCharacter);
	GetWorld()->GetTimerManager().SetTimer(UrnMaxHideTimerHandle, JumpOutTimerDel, MaxHideTime, false);

	// Gives a timer that allow player do manual jump out after minimum time
	GetWorld()->GetTimerManager().SetTimer(UrnMinHideTimerHandle, this, &AHidingUrn::AllowManualJumpOut, MinHideTime, false);

	// Gets and store the temporary information of the player
	TempPlayerInfo.PlayerMaxWalkSpeed = _playerCharacter->GetCharacterMovement()->MaxWalkSpeed;
	TempPlayerInfo.PlayerLocation = _playerCharacter->GetActorLocation();

	// Move the player away somewhere and make him invisible
	FVector TeleportLocation = this->GetActorLocation();
	TeleportLocation.Z += 200.0f;
	_playerCharacter->SetActorLocation(TeleportLocation);
	_playerCharacter->SetActorHiddenInGame(true);
	_playerCharacter->GetStimulusSourceComponent()->UnregisterFromSense(UAISense_Sight::StaticClass());

	// Disable collision and movement
	_playerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_playerCharacter->BlockMovementAction(true);
	_playerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0.0f;

	// Hide the outline
	UrnOutline->SetVisibility(false);

	// Re-register self as an interactable
	_playerCharacter->SetInteractionTarget(InteractableComponent);

	// Call the blueprint version of the function
	Receive_JumpIn(_playerCharacter);
}

void AHidingUrn::JumpOut(class APlayerCharacter* _playerCharacter)
{
	// Restore the movement of the player
	_playerCharacter->UnblockMovementInput();
	_playerCharacter->BlockMovementAction(false);
	_playerCharacter->GetCharacterMovement()->MaxWalkSpeed = TempPlayerInfo.PlayerMaxWalkSpeed;
	_playerCharacter->SetActorLocation(TempPlayerInfo.PlayerLocation);
	_playerCharacter->SetActorHiddenInGame(false);
	_playerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	_playerCharacter->GetStimulusSourceComponent()->RegisterForSense(UAISense_Sight::StaticClass());

	// Urn destruction
	FVector destructionLocation = UrnDestructableMesh->GetComponentLocation();
	UrnDestructableMesh->ApplyDamage(1.0f, destructionLocation, destructionLocation, 1.0f);

	// Disable the collision of the block volume
	BlockVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Call the blueprint version 
	Receive_JumpOut(_playerCharacter);

	// Clear all the timer handle
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// Disable the interaction after player has jumped out
	InteractableComponent->bCanInteract = false;
}

void AHidingUrn::AllowManualJumpOut()
{
	bAllowManualJumpOut = true;
	InteractableComponent->bCanInteract = true;
}

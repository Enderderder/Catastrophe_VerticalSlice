// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingTrigger.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"

// Sets default values
ALevelStreamingTrigger::ALevelStreamingTrigger()
{
 	// Set this actor to not call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelStreamingTrigger::OnPlayerEnterTrigger);
	RootComponent = TriggerBox;

	EditorOnlyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EditorOnlyMesh"));
	EditorOnlyMesh->bIsEditorOnly = true;
	EditorOnlyMesh->bHiddenInGame = true;
	EditorOnlyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EditorOnlyMesh->SetGenerateOverlapEvents(false);
	EditorOnlyMesh->SetupAttachment(TriggerBox);
}

// Called when the game starts or when spawned
void ALevelStreamingTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	// Do the check
	if (CurrentLevel == TEXT("DefaultName") ||
		DestinationLevel == TEXT("DefaultName"))
	{
		UE_LOG(LogTemp, Error,
			TEXT("Level Streaming Trigger Error:"
				"CurrentLevelName and DestinationLevelName should not be 'DefaultName'"));
	}
}

void ALevelStreamingTrigger::OnPlayerEnterTrigger(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")) && bActive)
	{
		PlayerToTeleport = OtherActor;

		if (LoadingScreenClass)
		{
			LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
			if (LoadingScreenWidget)
				LoadingScreenWidget->AddToViewport();
		}

		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.UUID = 0;
		latenInfo.Linkage = 0;
		latenInfo.ExecutionFunction = TEXT("OnLevelLoaded");
		UGameplayStatics::LoadStreamLevel(this, DestinationLevel, true, bShouldBlockOnLoad, latenInfo);
	}
}

void ALevelStreamingTrigger::OnLevelLoaded()
{
	if (bTeleportAfterLoaded && 
		PlayerToTeleport && 
		!PlayerToTeleport->IsPendingKill())
	{
		// We dont do scale transform cause it will mess with the character scale very badly
		FTransform teleportTransform = 
			DestinationTransformRelative + this->GetActorTransform();
		PlayerToTeleport->SetActorLocationAndRotation(
			teleportTransform.GetLocation(), teleportTransform.GetRotation());
	}

	if (bShouldUnloadThisLevelAfter)
	{
		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.ExecutionFunction = TEXT("OnLevelLoadingFullyFinished");
		latenInfo.UUID = 0;
		latenInfo.Linkage = 0;
		UGameplayStatics::UnloadStreamLevel(this, CurrentLevel, latenInfo, bShouldBlockOnLoad);
	}
	else
	{
		// just call the finishing function
		OnLevelLoadingFullyFinished();
	}
}

void ALevelStreamingTrigger::OnLevelLoadingFullyFinished()
{
	if (LoadingScreenWidget)
		LoadingScreenWidget->RemoveFromParent();
}

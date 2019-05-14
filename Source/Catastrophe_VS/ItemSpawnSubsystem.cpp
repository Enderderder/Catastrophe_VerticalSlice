// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnSubsystem.h"

#include "Interactable/ItemPickup.h"
#include "../../../UE_4.22/Engine/Source/Runtime/Engine/Classes/Engine/World.h"

// Initialise
UItemSpawnSubsystem::UItemSpawnSubsystem() : UCatastropheGameInstanceSubsystem()
{
	
}

void UItemSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UItemSpawnSubsystem::PostInitialize()
{
	Super::PostInitialize();

}

void UItemSpawnSubsystem::Deinitialize()
{
	Super::Deinitialize();

}

void UItemSpawnSubsystem::AddItemsList(TArray<class AItemPickup> _newItems)
{
	for (auto& item : _newItems)
	{
		ItemList.Add(item);
	}
}

void UItemSpawnSubsystem::RemoveItem(class AItemPickup _item)
{
	ItemList.Remove(_item);
}

void UItemSpawnSubsystem::SpawnItemAtLocation(FVector _location, AItemPickup* _itemClass)
{
	FRotator rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawnInfo;

	AItemPickup* newItem = GetWorld()->SpawnActor<AItemPickup>(GetClass(), _location, rotation);
}

void UItemSpawnSubsystem::RandomlySpawnItem()
{
	// Randomly choose item to spawn
	AItemPickup* randItem;
	randItem = ItemList.FindByKey(FMath::RandRange(0, ItemList.Num - 1));

	// Randomly choose location to spawn item at
	FVector randLocation;

	SpawnItemAtLocation(randLocation, randItem);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnSubsystem.h"

#include "Interactable/ItemPickup.h"
#include "Engine/World.h"

UItemSpawnSubsystem::UItemSpawnSubsystem() 
	: UCatastropheGameInstanceSubsystem()
{}

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

void UItemSpawnSubsystem::AddItemsToList(TArray<TSubclassOf<AItemPickup>> _newItems)
{
	// Adds new items to ItemList
	for (auto& item : _newItems)
	{
		ItemList.Add(item);
	}
}

void UItemSpawnSubsystem::AddItemToList(TSubclassOf<AItemPickup> _newItem)
{
	ItemList.Add(_newItem);
}

void UItemSpawnSubsystem::RemoveItem(TSubclassOf<AItemPickup> _item)
{
	// Removes selected item from ItemList
	ItemList.Remove(_item);
}

void UItemSpawnSubsystem::SetSpawnLocations(TArray<FVector> _newLocations)
{
	// Resets spawn location list
	SpawnLocationsList.Empty();

	// Adds new spawn locations to SpawnLocationList
	for (auto& location : _newLocations)
	{
		SpawnLocationsList.Add(location);
	}
}

AItemPickup* UItemSpawnSubsystem::SpawnItemAtLocation(FVector _location, TSubclassOf<AItemPickup> _itemClass)
{
	// Setting rotation values of item to spawn
	FRotator rotation(0.0f, 0.0f, 0.0f);

	// Spawn item
	AItemPickup* newItem = GetWorld()->SpawnActor<AItemPickup>(_itemClass, _location, rotation);
	return newItem;
}

AItemPickup* UItemSpawnSubsystem::RandomlySpawnItem()
{
	// Randomly choose item to spawn
	TSubclassOf<AItemPickup> randItem;
	randItem = ItemList[FMath::RandRange(0, ItemList.Num() - 1)];

	// Randomly choose location to spawn item at
	FVector randLocation;
	if (SpawnLocationsList.Num() > 0)
	{
		randLocation = SpawnLocationsList[FMath::RandRange(0, SpawnLocationsList.Num() - 1)];
	}

	// Calls function which spawns item
	return SpawnItemAtLocation(randLocation, randItem);
}
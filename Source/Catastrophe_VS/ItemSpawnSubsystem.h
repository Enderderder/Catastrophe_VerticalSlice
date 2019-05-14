// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "ItemSpawnSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API UItemSpawnSubsystem : public UCatastropheGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UItemSpawnSubsystem();

protected:
	TArray<class AItemPickup> ItemList;
	TArray<FVector> SpawnLocationsList;

public:
	/* Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/* Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/* Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	/**
	 * Adds ItemList to selected items
	 * @param Array of new items to add
	 * @note This will add items to the item list of possible items to spawn
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Spawning")
	void AddItemsList(TArray<class AItemPickup> _newItems);

	/**
	 * Removes selected item from ItemList
	 * @param Items to remove
	 * @note This will remove a selected item from the item list of possible items to spawn
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Spawning")
	void RemoveItem(class AItemPickup _item);

	/**
	 * Sets locations in ItemList
	 * @param Array of new items to add
	 * @note This will add items to the list of possible items to spawn
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Spawning")
	void SetSpawnLocations(TArray<FVector> _newLocations);

	/**
	 * Spawns an item at a selected position
	 * @param Spawn Location, item type to spawn
	 * @note This will spawn a selected item type at a selected location
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Spawning")
	void SpawnItemAtLocation(FVector _location, class AItemPickup* _itemClass);

	/**
	 * Randomly choose location and item and spawn it
	 * @param None
	 * @note This will randomly choose a location and item to spawn and spawn it
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Spawning")
	void RandomlySpawnItem();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "SaveGameSubSystem.generated.h"

#define SLOT_NAME_BASE "Slot"
#define MAX_SLOT_NUM 5

// Delegate that has one parameter
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSaveSystemDelegateOneParam, class UCatastropheSaveGame*, _saveGameInst);

/**
 * This is a GameInstanceSubSystem that manage multiple game progress
 * Save and load different data
 */
UCLASS()
class CATASTROPHE_VS_API USaveGameSubsystem : public UCatastropheGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USaveGameSubsystem();

	/** Delegate variable that broadcast when there is a save game successfully loaded */
	UPROPERTY(BlueprintAssignable, Category = "SaveGameSystem")
	FSaveSystemDelegateOneParam OnSavedGameLoaded;

protected:

	/** The save game instance that is currently loaded and all change will write on top of it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGameSystem")
	class UCatastropheSaveGame* LoadedSaveGameInst;

public:

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	/** Creates a save game profile by adding it do certain slot */
	UFUNCTION(BlueprintCallable, Category = "SaveGameSystem")
	void CreateSavedGame(int32 _slotIndex);

	/**
	 * Loads the save game instance using the name and index
	 * @param Slot index of the save game
	 * @note This will overwrite the currently loaded save game
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveGameSystem")
	void LoadSavedGame(int32 _slotIndex);

	/**
	 * Delete a saved game profile using slot index
	 * @param Slot index of the save game
	 * @note Don't call this function when there is any saved game loaded
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveGameSystem")
	void DeleteSavedGameByIndex(int32 _slotIndex);

	/**
	 * Delete a saved game profile using slot name
	 * @param Slot name of the save game
	 * @note Don't call this function when there is any saved game loaded
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveGameSystem")
	void DeleteSavedGameByName(FString _slotName);

	/**
	 * Delete all saved game
	 * @note Use with caution
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveGameSystem")
	void DeleteAllSavedGame();

	/** Check if there is any existing saved game in the slot */
	UFUNCTION(BlueprintCallable, Category = "SaveGameSystem")
	bool DoesSlotHasSavedGame(int32 _slotIndex) const;

	/** Getter */
	FORCEINLINE class UCatastropheSaveGame* GetLoadedSaveGameInst() const { 
		return LoadedSaveGameInst; 
	}
	/** Getter End */

	/** Gets the instance without going through the GameInstance */
	static USaveGameSubsystem* GetInst(const UObject* _worldContextObject);

protected:




};

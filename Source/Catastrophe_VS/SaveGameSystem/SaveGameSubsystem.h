// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubSystem.generated.h"

#define SLOT_NAME_BASE "Slot"
#define MAX_SLOT_NUM 5

/**
 * This is a GameInstanceSubSystem that manage multiple game progress
 * Save and load different data
 */
UCLASS()
class CATASTROPHE_VS_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USaveGameSubsystem();

protected:

	/** The save game instance that is currently loaded and all change will write on top of it */
	class UCatastropheSaveGame* LoadedSaveGameInst;

public:

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	/** Loads the save game instance using the name and index */
	void LoadSavedGame(int32 _slotNum);

protected:


};

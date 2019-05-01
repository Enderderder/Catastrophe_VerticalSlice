// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CatastropheSaveGame.generated.h"

// Try forward declare?
enum class EQuestState : uint8;

/**
 * This is a save game instance for the game Catastrophe
 */
UCLASS()
class CATASTROPHE_VS_API UCatastropheSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Set default values for the class
	UCatastropheSaveGame();

	/** The name of the slot this save game instance will be saved in */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	FString SaveGameSlotName;

	/** The user index of the save game instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	int32 UserIndex;

	/** The quest state of all the quest in the game */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TMap<int32, EQuestState> SavedQuestState;




};

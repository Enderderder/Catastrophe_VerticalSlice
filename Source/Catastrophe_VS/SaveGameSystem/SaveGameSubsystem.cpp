// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

#include "Catastrophe_VS.h"
#include "CatastropheSaveGame.h"
#include "QuestSystem/QuestSubsystem.h"
#include "QuestSystem/Quest.h"

// Set default value
USaveGameSubsystem::USaveGameSubsystem()
	: UCatastropheGameInstanceSubsystem()
{
	LoadedSaveGameInst = nullptr;
}

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void USaveGameSubsystem::PostInitialize()
{
	Super::PostInitialize();


}

void USaveGameSubsystem::Deinitialize()
{
	Super::Deinitialize();

}

void USaveGameSubsystem::CreateSavedGame(int32 _slotIndex)
{
	// Check if the index is in acceptable range
	if (_slotIndex < 0 || _slotIndex > MAX_SLOT_NUM)
	{
		UE_LOG(LogSaveGameSystem, Warning,
			TEXT("Slot number out of bound, acceptable slot number is between 0 and %s"),
			*FString::FromInt(MAX_SLOT_NUM));
		return;
	}

	// Forge the name of the slot
	FString slotName = SLOT_NAME_BASE + FString::FromInt(_slotIndex);

	// Check if there is already a saved game exists in the slot
	if (UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		UE_LOG(LogSaveGameSystem, Warning,
			TEXT("The slot is been used, try another slot"));
		return;
	}

	// Create the save game instance and initialize it
	UCatastropheSaveGame* savedGame = Cast<UCatastropheSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UCatastropheSaveGame::StaticClass()));
	savedGame->SaveGameSlotName = slotName;
	savedGame->InitializeSaveGameInst(GetGameInstance());
	UGameplayStatics::SaveGameToSlot(savedGame, slotName, 0);
}

void USaveGameSubsystem::LoadSavedGame(int32 _slotIndex)
{
	// Check if the index is in acceptable range
	if (_slotIndex < 0 || _slotIndex > MAX_SLOT_NUM)
	{
		UE_LOG(LogSaveGameSystem, Warning,
			TEXT("Slot number out of bound, acceptable slot number is between 0 and %s"),
			*FString::FromInt(MAX_SLOT_NUM));
		return;
	}

	// Forge the name of the slot
	FString slotName = SLOT_NAME_BASE + FString::FromInt(_slotIndex);

	// Check to see if this slot has data saved in it
	if (!UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		UE_LOG(LogSaveGameSystem, Warning,
			TEXT("Cannot load saved game from %s, slot is empty, make sure create the a saved game first"),
			*slotName);
		return;
	}
	
	// Load the active
	UCatastropheSaveGame* saveGameInst = Cast<UCatastropheSaveGame>(
			UGameplayStatics::LoadGameFromSlot(slotName, 0));
	if (!saveGameInst)
	{
		UE_LOG(LogSaveGameSystem, Warning, 
			TEXT("Cannot get the save game instance by type: UCatastropheSaveGame"));
		return;
	}
	LoadedSaveGameInst = saveGameInst;

	// Broadcast this message
	OnSavedGameLoaded.Broadcast(LoadedSaveGameInst);
}

void USaveGameSubsystem::DeleteSavedGameByIndex(int32 _slotIndex)
{
	// Forge the name of the slot
	FString slotName = SLOT_NAME_BASE + FString::FromInt(_slotIndex);

	DeleteSavedGameByName(slotName);
}

void USaveGameSubsystem::DeleteSavedGameByName(FString _slotName)
{
	// Check to see if the save game in the slot actually exist
	if (!UGameplayStatics::DoesSaveGameExist(_slotName, 0))
	{
		UE_LOG(LogSaveGameSystem, Warning,
			TEXT("Cannot delete the save game, there is no data saved in the slot"));
		return;	
	}

	// Check if the save game is running
	if (LoadedSaveGameInst && LoadedSaveGameInst->SaveGameSlotName == _slotName)
	{
		UE_LOG(LogSaveGameSystem, Warning,
			TEXT("Cannot delete the save game while it is loaded, unload them first"));
		return;
	}

	UGameplayStatics::DeleteGameInSlot(_slotName, 0);
}

void USaveGameSubsystem::DeleteAllSavedGame()
{
	// If theres loaded save game instance running, should unload it first
	if (LoadedSaveGameInst)
	{
		UE_LOG(LogSaveGameSystem, Warning,
			TEXT("Cannot delete the save game while it is loaded, unload them first"));
		return;
	}

	// Delete all saved game
	for (int32 index = 0; index < MAX_SLOT_NUM; ++index)
	{
		// Forge the name of the slot
		FString slotName = SLOT_NAME_BASE + FString::FromInt(index);

		// Check to see if the file actually exist before delete
		if (UGameplayStatics::DoesSaveGameExist(slotName, 0))
		{
			UGameplayStatics::DeleteGameInSlot(slotName, 0);
		}
	}
}

bool USaveGameSubsystem::DoesSlotHasSavedGame(int32 _slotIndex) const
{
	FString slotName = SLOT_NAME_BASE + FString::FromInt(_slotIndex);
	return UGameplayStatics::DoesSaveGameExist(slotName, 0);
}

USaveGameSubsystem* USaveGameSubsystem::GetInst(const UObject* _worldContextObject)
{
	// Try to get the game instance and search for the USaveGameSubsystem instance
	if (UGameInstance* GameInst
		= UGameplayStatics::GetGameInstance(_worldContextObject))
	{
		return GameInst->GetSubsystem<USaveGameSubsystem>();
	}
	
	return nullptr;
}

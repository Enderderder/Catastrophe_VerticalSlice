// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "CatastropheSaveGame.h"

USaveGameSubsystem::USaveGameSubsystem()
	: UGameInstanceSubsystem()
{
	LoadedSaveGameInst = nullptr;
}

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void USaveGameSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
}

void USaveGameSubsystem::LoadSavedGame(int32 _slotNum)
{
	if (_slotNum >= 0 && _slotNum < MAX_SLOT_NUM)
	{
		FString slotName = SLOT_NAME_BASE + FString::FromInt(_slotNum);

		// TODO
		// Load into the instance from UGameplayStatic


	}
	else
	{
		// TODO
		// UE_LOG
	}


	



}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/LevelStreaming.h"
#include "Engine/Level.h"

#include "StreamingLevelInterface.h"

URespawnSubsystem::URespawnSubsystem() 
	: UCatastropheGameInstanceSubsystem()
{}

void URespawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void URespawnSubsystem::PostInitialize()
{
	Super::PostInitialize();

	

}

void URespawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
}

void URespawnSubsystem::LoadLevelStreaming(const UObject* _worldContextObject, FLoadStreamingLevelInfo _loadLevelInfo)
{
	// Store the temp value
	tempInfo = _loadLevelInfo;

	FLatentActionInfo latenInfo;
	latenInfo.CallbackTarget = this;
	latenInfo.UUID = 0;
	latenInfo.Linkage = 0;
	latenInfo.ExecutionFunction = TEXT("OnLevelLoaded");

	UGameplayStatics::LoadStreamLevel(
		_worldContextObject,
		_loadLevelInfo.LoadedLevelName,
		true,
		_loadLevelInfo.bBlockOnLoad,
		latenInfo);
}

URespawnSubsystem* URespawnSubsystem::GetInst(const UObject* _worldContextObject)
{
	if (UGameInstance * gameInst
		= UGameplayStatics::GetGameInstance(_worldContextObject))
	{
		return gameInst->GetSubsystem<URespawnSubsystem>();
	}
	return nullptr;
}

void URespawnSubsystem::OnLevelLoaded()
{
	ULevelStreaming* loadedLevelStream = 
		UGameplayStatics::GetStreamingLevel(this, tempInfo.LoadedLevelName);
	if (loadedLevelStream)
	{
		if (ULevel* loadedLevel = loadedLevelStream->GetLoadedLevel())
		{
			if (loadedLevel->GetClass()->ImplementsInterface(UStreamingLevelInterface::StaticClass()))
			{
				IStreamingLevelInterface::Execute_OnStreamLevelLoaded(loadedLevel, tempInfo);
			}
		}
	}
}

void URespawnSubsystem::OnLevelUnloaded()
{

}

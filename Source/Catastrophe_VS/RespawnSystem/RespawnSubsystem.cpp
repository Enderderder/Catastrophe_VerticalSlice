// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnSubsystem.h"

#include "GameFramework/Character.h"
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
	
	// Add all the locations to the map
	for (int32 i = (int32)EDISTRICT::HUB; i < (int32)EDISTRICT::LOCATIONCOUNT; ++i)
	{
		EDISTRICT districtType = static_cast<EDISTRICT>(i);
		RespawnPoints.Add(districtType);
		RespawnPoints[districtType].District = districtType;
	}
}

void URespawnSubsystem::PostInitialize()
{
	Super::PostInitialize();

}

void URespawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
}

void URespawnSubsystem::LoadLevelStreaming(FLoadStreamingLevelInfo _loadLevelInfo)
{
	// Store the temp value
	tempInfo = _loadLevelInfo;

	FLatentActionInfo latenInfo;
	latenInfo.CallbackTarget = this;
	latenInfo.UUID = 1;
	latenInfo.Linkage = 0;
	latenInfo.ExecutionFunction = TEXT("OnStreamLevelLoaded");

	UGameplayStatics::LoadStreamLevel(
		this,
		_loadLevelInfo.LoadedLevelName,
		true,
		_loadLevelInfo.bBlockOnLoad,
		latenInfo);
}

void URespawnSubsystem::UnloadStreamingLevel(FName _levelName)
{
	FLatentActionInfo latenInfo;
	UGameplayStatics::UnloadStreamLevel(this, _levelName, latenInfo, false);
}

void URespawnSubsystem::RegisterRespawnLocation(EDISTRICT _districtType, FTransform _transform)
{
	// Check if the district is valid
	if ((int32)_districtType < 0 || (int32)_districtType >= (int32)EDISTRICT::LOCATIONCOUNT)
	{
		UE_LOG(LogTemp, Error, 
			TEXT("Failed to register respawn point because the district type is invalid"));
		return;
	}

	// Force the scale to be 1 so the player respawn will never messed up
	_transform.SetScale3D(FVector::OneVector);

	// Store the location
	RespawnPoints[_districtType].RespawnTransforms.Add(_transform);
}

FTransform URespawnSubsystem::GetFirstRespawnLocationAtDistrict(EDISTRICT _districtType)
{
	// Check if the district is valid
	if ((int32)_districtType < 0 || (int32)_districtType >= (int32)EDISTRICT::LOCATIONCOUNT)
	{
		UE_LOG(LogTemp, Error,
			TEXT("Unable to find respawn location cause the district type is invalid"));
		return FTransform::Identity;
	}

	// Check if there is any locations registered
	TArray<FTransform> respawnPointsInDistrict = RespawnPoints[_districtType].RespawnTransforms;
	if (respawnPointsInDistrict.Num() <= 0)
	{
		UE_LOG(LogTemp, Error,
			TEXT("Unable to find respawn location cause there is no location in district"));
		return FTransform::Identity;
	}

	// Give the transform
	return respawnPointsInDistrict[0];
}

void URespawnSubsystem::RespawnPlayerAtLocation(EDISTRICT _districtType)
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (player && !player->IsPendingKill())
	{
		FTransform location = GetFirstRespawnLocationAtDistrict(_districtType);
		player->SetActorTransform(location);
	}
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

FName URespawnSubsystem::GetStreamingLevelNameFromActor(AActor* _actor)
{
	if (_actor && !_actor->IsPendingKill())
	{
		return _actor->GetLevel()->GetOuter()->GetFName();
	}

	return NAME_None;
}

void URespawnSubsystem::OnStreamLevelLoaded()
{
	if (tempInfo.bTeleportPlayer)
	{
		RespawnPlayerAtLocation(tempInfo.DistrictType);
	}

	if (tempInfo.bUnloadCurrentLevel)
	{
		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.UUID = 1;
		latenInfo.Linkage = 0;
		latenInfo.ExecutionFunction = TEXT("OnStreamLevelUnloaded");

		UGameplayStatics::UnloadStreamLevel(
			this, 
			tempInfo.OriginalLevelName, 
			latenInfo, 
			tempInfo.bBlockOnLoad);
	}
}

void URespawnSubsystem::OnStreamLevelUnloaded()
{
	
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "RespawnSubsystem.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EDISTRICT : uint8
{
	HUB = 0,
	MARKET,
	HOLDINGCELL,
	JAIL,

	LOCATIONCOUNT // This should always be the last
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLoadStreamingLevelInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OriginalLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LoadedLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnloadCurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTeleportPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDISTRICT DistrictType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBlockOnLoad;

	FLoadStreamingLevelInfo() :
		OriginalLevelName(TEXT("DefaultName")),
		LoadedLevelName(TEXT("DefaultName")),
		bUnloadCurrentLevel(true),
		bTeleportPlayer(true),
		DistrictType(EDISTRICT::HUB),
		bBlockOnLoad(false)
	{}
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FRespawnLocation
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDISTRICT District;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> RespawnTransforms;

	FRespawnLocation() :
		District(EDISTRICT::HUB)
	{}
};

/**
 * This system controls the spawn of the player
 */
UCLASS()
class CATASTROPHE_VS_API URespawnSubsystem : public UCatastropheGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:

	/** This is a temporary info store */
	FLoadStreamingLevelInfo tempInfo;

public:
	URespawnSubsystem();

protected:

	/** All the respawn locations that gets registered */
	UPROPERTY()
	TMap<EDISTRICT, FRespawnLocation> RespawnPoints;

public:
	/* Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/* Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/* Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	/**
	 * Load the level inside the level streaming world
	 * @param _player The player reference that needs to be provided
	 * @param _loadLevelInfo Information that needs to be filled in order to load level
	 * @note Do not call this function before all BeginPlay() finished
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void LoadLevelStreaming(FLoadStreamingLevelInfo _loadLevelInfo);

	/**
	 * Called to store spawn locations at certain district
	 * @param _districtType
	 * @param _transform
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RegisterRespawnLocation(EDISTRICT _districtType, FTransform _transform);

	/**
	 * Gets the first respawn location at provided district type
	 * @param _districtType
	 * @note If there is no respawn location avaliable, return zero transform
	 * @note Cause this will be the repsawn transform, the scale will forced to set to 1
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	FTransform GetFirstRespawnLocationAtDistrict(EDISTRICT _districtType);

	/**
	 * Relocate the player to a district
	 * @param _districtType The location player is going to move to
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RespawnPlayerAtLocation(EDISTRICT _districtType);

	/** Gets the instance without going through the GameInstance */
	static URespawnSubsystem* GetInst(const UObject* _worldContextObject);

protected:

	/** Called when an level is loaded */
	UFUNCTION()
	void OnStreamLevelLoaded();

	/** Called when an level is unloaded */
	UFUNCTION()
	void OnStreamLevelUnloaded();

};

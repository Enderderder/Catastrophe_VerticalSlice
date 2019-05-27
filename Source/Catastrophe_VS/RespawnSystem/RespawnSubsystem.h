// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "RespawnSubsystem.generated.h"

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
	bool bBlockOnLoad;

	FLoadStreamingLevelInfo() :
		OriginalLevelName(TEXT("DefaultName")),
		LoadedLevelName(TEXT("DefaultName")),
		bUnloadCurrentLevel(true),
		bTeleportPlayer(true),
		bBlockOnLoad(false)
	{}
};

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
struct FRespawnLocation
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDISTRICT District;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> RespawnLocations;

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

	//TMap()

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
	UFUNCTION(BlueprintCallable)
	void LoadLevelStreaming(const UObject* _worldContextObject, FLoadStreamingLevelInfo _loadLevelInfo);

	/** Gets the instance without going through the GameInstance */
	static URespawnSubsystem* GetInst(const UObject* _worldContextObject);

protected:

	/** Called when an level is loaded */
	void OnLevelLoaded();

	/** Called when an level is unloaded */
	void OnLevelUnloaded();

};

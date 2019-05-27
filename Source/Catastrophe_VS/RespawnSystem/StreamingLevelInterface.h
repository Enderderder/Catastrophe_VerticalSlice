// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StreamingLevelInterface.generated.h"

struct FLoadStreamingLevelInfo;


// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UStreamingLevelInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface is designed to implement in level blueprints
 */
class CATASTROPHE_VS_API IStreamingLevelInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * Called when the levels loaded by the respawn system
	 * @param _levelLoadedInfo The information of the level loading
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "LevelStreaming")
	void OnLevelLoaded(FLoadStreamingLevelInfo _levelLoadedInfo);
	virtual void OnLevelLoaded_Implementation(FLoadStreamingLevelInfo _levelLoadedInfo) = 0;

	/**
	 * Called when the levels unloaded by the respawn system
	 * @param _levelLoadedInfo The information of the level loading
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "LevelStreaming")
	void OnLevelUnloaded(FLoadStreamingLevelInfo _levelLoadedInfo);
	virtual void OnLevelUnloaded_Implementation(FLoadStreamingLevelInfo _levelLoadedInfo) = 0;

};

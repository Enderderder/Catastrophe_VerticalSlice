// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CatastropheGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API UCatastropheGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCatastropheGameInstance(const FObjectInitializer& ObjectInitializer);

	/** Temp | Store where to spawn in the hub level */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Temp")
	FString HubSpawnLocation;

public:
	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init() override;

	/** virtual function to allow custom GameInstances an opportunity to do cleanup when shutting down */
	virtual void Shutdown() override;

};

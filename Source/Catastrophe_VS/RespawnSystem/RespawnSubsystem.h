// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "RespawnSubsystem.generated.h"

/**
 * This system controls the spawn of the player
 */
UCLASS()
class CATASTROPHE_VS_API URespawnSubsystem : public UCatastropheGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	URespawnSubsystem();

	/* Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/* Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/* Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;



	/** Gets the instance without going through the GameInstance */
	static URespawnSubsystem* GetInst(const UObject* _worldContextObject);



};

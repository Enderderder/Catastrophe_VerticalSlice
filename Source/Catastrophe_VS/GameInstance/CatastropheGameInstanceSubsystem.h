// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CatastropheGameInstanceSubsystem.generated.h"

/**
 * This is an advance and customized version of the game instance subsystem
 * Contain some extra functionality which should be used for the game
 */
UCLASS(Abstract)
class CATASTROPHE_VS_API UCatastropheGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCatastropheGameInstanceSubsystem();

	/** Implement this for post initilization after all subsystem has initialized */
	UFUNCTION()
	virtual void PostInitialize();

	/// There is no reason to link blueprint subsytem yet
	/** Blueprint version of the PostInitialization() */
	/*UFUNCTION(BlueprintImplementableEvent, Category = "GameInstanceSubsystem",
		meta = (DisplayName = "PostInitialize"))
	void Receive_PostInitialize();*/

protected:

	/** Get another subsystem by its type through a subsystem */
	template <typename TSubsystemClass>
	TSubsystemClass* GetOtherSubsytem() const 
	{
		UGameInstance* inst = GetGameInstance();
		return GetGameInstance()->GetSubsystem<TSubsystemClass>();
	}

};

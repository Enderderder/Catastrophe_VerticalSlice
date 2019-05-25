// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CatastropheMainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API ACatastropheMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/** Transition to all actors beginplay */
	virtual void StartPlay() override;

};

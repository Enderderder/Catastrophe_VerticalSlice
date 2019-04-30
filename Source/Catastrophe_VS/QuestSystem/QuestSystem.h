// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSystem.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API UQuestSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UQuestSystem();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	class UDataTable* QuestDT;
	

public:

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

protected:

	/** Loads all the data from data table into struct array */
	void LoadQuestDataTableToArray(OUT TArray<struct FQuestInfo>& _outArray);


};

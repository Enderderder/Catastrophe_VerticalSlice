// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

/** Forward Delare */
class UQuest;
struct FQuestInfo;

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UQuestSubsystem();

protected:

	/** Data table that contains the information of all the quests */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	class UDataTable* QuestDT;
	
	/** The array that contains all the quest thats been load */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TArray<UQuest*> Quests;

public:

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	/**
	 * Call to get the quest class
	 * @param The name of the quest
	 * @return The UQuest class object
	 */
	class UQuest* GetQuest(FString _name) const;

	/**
	 * Call to get the quest class
	 * @param The ID of the quest
	 * @return The UQuest class object
	 */
	class UQuest* GetQuest(int32 _id) const;

protected:

	/** Loads all the data from data table into struct array */
	void LoadQuestDataTableToArray(OUT TArray<FQuestInfo>& _outArray);


};

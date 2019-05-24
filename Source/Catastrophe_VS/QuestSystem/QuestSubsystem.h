// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

/** Forward Declare */
class UQuest;
struct FQuestInfo;

/**
 * The quest system of the game that loads quest by data table
 */
UCLASS()
class CATASTROPHE_VS_API UQuestSubsystem : public UCatastropheGameInstanceSubsystem
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

	/** Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	/** Delegate binding function, receive call when a new save is loaded */
	UFUNCTION()
	void OnSaveGameLoaded(class UCatastropheSaveGame* _saveGameInst);

	/** Register objective into the objective slot */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void RegisterObjectiveToQuest(class UQuestObjectiveComponent* _objective, int32 _questID);

	/**
	 * Call to get the quest class
	 * @param The name of the quest
	 * @return The UQuest class object
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	UQuest* GetQuestByName(FString _name) const;

	/**
	 * Call to get the quest class
	 * @param The ID of the quest
	 * @return The UQuest class object
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	UQuest* GetQuestByID(int32 _id) const;

	/** Getter */
	FORCEINLINE TArray<UQuest*> GetAllQuests() const { return Quests; }
	FORCEINLINE int32 GetTotalQuestCount() const { return Quests.Num(); }
	/** Getter End */

	/** Gets the instance without going through the GameInstance */
	static UQuestSubsystem* GetInst(const UObject* _worldContextObject);

protected:

	/** Loads all the data from data table into struct array */
	void LoadQuestDataTableToArray(OUT TArray<FQuestInfo>& _outArray);


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Quest.generated.h"

/**
 * An enum which define the state of the quest
 */
UENUM(BlueprintType)
enum class EQuestState : uint8
{
	Locked = 0,
	Avaliable,
	Active,
	Completed,

	StateCount // This is the counter, should always be at the end
};

/**
 * A struct that contains information of the quest
 */
USTRUCT(BlueprintType)
struct FQuestInfo : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RewardExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> ChildQuestIDs;

	FQuestInfo() :
		QuestID(0),
		QuestName("DefaultName"),
		QuestDescription("DefaultDescription"),
		RewardExp(0)
	{}
};

/**
 * This object class contains all the information of a quest
 * Quest functionality is also included in here
 */
UCLASS()
class CATASTROPHE_VS_API UQuest : public UObject
{
	GENERATED_BODY()
	
public:
	// Default Constructor
	UQuest();

protected:
	
	/** The Information of the quest */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestSystem")
	FQuestInfo QuestInfo;

	/** The state of the quest */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestSystem")
	EQuestState QuestState = EQuestState::Locked;

	/** The objectives that need to be acheive to complete the quest */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestSystem")
	TArray<class UQuestObjectiveComponent*> QuestObjectives;

	/** Quest Tree */

	/** Contains all the parent quest of this quest */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestSystem")
	TArray<UQuest*> ParentQuests;

	/** Contains all the child quest of this quest */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestSystem")
	TArray<UQuest*> ChildQuests;

	/** QUest Tree End */


public:

	/** Loads the data for the quest */
	void SetQuestData(FQuestInfo _questInfo);

	/** Set the child quest */
	void AppendChildQuest(UQuest* _childQuest);

	/** Append a parent quest in here */
	void AppendParentQuest(UQuest* _parentQuest);

	/** Called when the quest that the objective has completed */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void CompleteObjective(class UQuestObjectiveComponent* _objective);

	/** Set the state of the quest */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void SetQuestState(EQuestState _questState);

	/** Register objective into the objective slot */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void RegisterObjective(class UQuestObjectiveComponent* _objective);

	/** Getter */
	FORCEINLINE TArray<class UQuestObjectiveComponent*> GetObjectives() const { return QuestObjectives; }
	FORCEINLINE FQuestInfo GetQuestInfo() const { return QuestInfo; }
	FORCEINLINE EQuestState GetState() const { return QuestState; }
	class UQuestSubsystem* GetQuestSystem() const;
	/** Getter End */

	
private:

	/** Quest action | Unlock */
	void UnlockQuest();

	/** Quest action | Activate */
	void ActivateQuest();

	/** Quest action | Complete */
	void CompleteQuest();


};

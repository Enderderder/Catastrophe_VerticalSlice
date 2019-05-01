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
	
	// The Information of the quest
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	FQuestInfo QuestInfo;

	// The state of the quest
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest")
	EQuestState QuestState;


	/** Quest Tree */

	// Contains all the parent quest of this quest
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<UQuest*> ParentQuests;

	// Contains all the child quest of this quest
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<UQuest*> ChildQuests;

	/** QUest Tree End */


public:

	/** Loads the data for the quest */
	void LoadQuestData(FQuestInfo _questInfo);

	/** Set the state of the quest */
	void SetQuestState(EQuestState _questState);

	/** Getter */
	FORCEINLINE FQuestInfo GetQuestInfo() const { return QuestInfo; }
	FORCEINLINE EQuestState GetState() const { return QuestState; }
	class UQuestSubsystem* GetQuestSystem() const;
	/** Getter End */

};

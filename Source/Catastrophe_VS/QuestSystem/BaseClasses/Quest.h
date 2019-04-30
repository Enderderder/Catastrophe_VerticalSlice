// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.generated.h"

/**
 * An enum which define the state of the quest
 */
UENUM(BlueprintType)
enum EQuestState
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
struct FQuestInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
		FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RewardExp;

	FQuestInfo() :
		QuestName("DefaultName"),
		QuestDescription("DefaultDescription"),
		RewardExp(0)
	{}
};

/**
 * This object class contains all the information of a quest
 * Quest functionality is also included in here
 */
UCLASS(Blueprintable)
class CATASTROPHE_VS_API UQuest : public UObject
{
	GENERATED_BODY()
	
public:
	// Default Constructor
	UQuest();


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FQuestInfo QuestInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest")
	TEnumAsByte<EQuestState> QuestState;

public:

	// Sets the Quest State
	void SetQuestState(EQuestState _questState);

	/** Getter */
	FORCEINLINE FQuestInfo GetQuestInfo() const { return QuestInfo; }
	FORCEINLINE TEnumAsByte<EQuestState> GetState() const { return QuestState; }
	/** Getter End */

};

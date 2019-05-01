// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubsystem.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

#include "Catastrophe_VS.h"
#include "Quest.h"

UQuestSubsystem::UQuestSubsystem()
	: UGameInstanceSubsystem()
{
	// Locate the data table in the content folder
	static ConstructorHelpers::FObjectFinder<UDataTable> QuestDataTableObject(
		TEXT("DataTable'/Game/Blueprints/Quests/DT_Quests.DT_Quests'"));
	if (QuestDataTableObject.Succeeded())
	{
		QuestDT = QuestDataTableObject.Object;
	}
	else
	{
		UE_LOG(LogQuestSystem, Warning,
			TEXT("Cannot locate QuestDataTableObject, check content folder path"));
	}
}

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load the data table into array
	TArray<FQuestInfo> QuestInfos;
	LoadQuestDataTableToArray(QuestInfos);

	// Create quest object and load quest info into them
	Quests.SetNum(QuestInfos.Num());
	for (int32 index = 0; index < Quests.Num(); ++index)
	{
		UQuest* quest = NewObject<UQuest>(this);
		quest->LoadQuestData(QuestInfos[index]);
		Quests[index] = quest;
	}
}

void UQuestSubsystem::Deinitialize()
{
	Super::Deinitialize();
	

}

UQuest* UQuestSubsystem::GetQuest(FString _name) const
{
	for (UQuest* quest : Quests)
	{
		if (quest->GetQuestInfo().QuestName == _name)
		{
			return quest;
		}
	}

	// When the quest cannot be find
	UE_LOG(LogQuestSystem, Warning,
		TEXT("No quest can be find by name: %s"), *_name);
	return nullptr;
}

UQuest* UQuestSubsystem::GetQuest(int32 _id) const
{
	for (UQuest* quest : Quests)
	{
		if (quest->GetQuestInfo().QuestID == _id)
		{
			return quest;
		}
	}

	// When the quest cannot be find
	UE_LOG(LogQuestSystem, Warning,
		TEXT("No quest can be find by ID: %s"), _id);
	return nullptr;
}

void UQuestSubsystem::LoadQuestDataTableToArray(OUT TArray<struct FQuestInfo>& _outArray)
{
	FString contextString;
	TArray<FQuestInfo*> tempArray;

	QuestDT->GetAllRows<FQuestInfo>(TEXT(""), tempArray);

	for (auto it : tempArray)
	{
		_outArray.Add(*it);
	}
}

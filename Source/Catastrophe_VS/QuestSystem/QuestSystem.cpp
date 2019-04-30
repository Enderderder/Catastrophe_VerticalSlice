// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

#include "Quest.h"

UQuestSystem::UQuestSystem()
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
		UE_LOG(LogTemp, Warning,
			TEXT("Cannot locate QuestDataTableObject, check content folder path"));
	}
}

void UQuestSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TArray<FQuestInfo> QuestInfos;
	LoadQuestDataTableToArray(QuestInfos);
}

void UQuestSystem::Deinitialize()
{
	Super::Deinitialize();
	

}

void UQuestSystem::LoadQuestDataTableToArray(OUT TArray<struct FQuestInfo>& _outArray)
{
	FString contextString;
	TArray<FQuestInfo*> tempArray;

	QuestDT->GetAllRows<FQuestInfo>(TEXT(""), tempArray);

	for (auto it : tempArray)
	{
		_outArray.Add(*it);
	}
}

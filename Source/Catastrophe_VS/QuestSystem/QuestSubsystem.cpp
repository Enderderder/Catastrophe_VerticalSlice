// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubsystem.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

#include "Catastrophe_VS.h"
#include "Quest.h"
#include "SaveGameSystem/SaveGameSubsystem.h"
#include "SaveGameSystem/CatastropheSaveGame.h"

UQuestSubsystem::UQuestSubsystem()
	: UCatastropheGameInstanceSubsystem()
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
		quest->SetQuestData(QuestInfos[index]);
		Quests[index] = quest;
	}

	// Link the quest up
	for (UQuest* quest : Quests)
	{
		TArray<int32> idArray = quest->GetQuestInfo().ChildQuestIDs;
		for (int32 id : idArray)
		{
			quest->AppendChildQuest(GetQuestByID(id));
		}
	}
}

void UQuestSubsystem::PostInitialize()
{
	Super::PostInitialize();

	// Binds the delegate
	USaveGameSubsystem* saveGameSystem = USaveGameSubsystem::GetInst(this);
	saveGameSystem->OnSavedGameLoaded.AddDynamic(this, &UQuestSubsystem::OnSaveGameLoaded);
}

void UQuestSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
}

void UQuestSubsystem::OnSaveGameLoaded(class UCatastropheSaveGame* _saveGameInst)
{
	// Check to see if there's any number difference between saved game quest 
	// state and the loaded quest
	int32 saveGameInstQuestCount = _saveGameInst->SavedQuestState.Num();
	if (saveGameInstQuestCount != Quests.Num())
	{
		// Might need to delete the saved game manually and recreate them
		UE_LOG(LogQuestSystem, Error,
			TEXT("The saved game quest count is not the same as the total quest count"));
		return;
	}

	// Load the state of the quest from the saved game
	for (int32 index = 0; index < saveGameInstQuestCount; ++index)
	{
		EQuestState questState = _saveGameInst->SavedQuestState[index];
		Quests[index]->SetQuestState(questState);
	}

	// Always make the root of the quest available or in other state
	if (Quests[0]->GetState() == EQuestState::Locked)
	{
		Quests[0]->SetQuestState(EQuestState::Avaliable);
	}
}

void UQuestSubsystem::RegisterObjectiveToQuest(class UQuestObjectiveComponent* _objective, int32 _questID)
{
	UQuest* quest = GetQuestByID(_questID);
	quest->RegisterObjective(_objective);
}

UQuest* UQuestSubsystem::GetQuestByName(FString _name) const
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

UQuest* UQuestSubsystem::GetQuestByID(int32 _id) const
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

UQuestSubsystem* UQuestSubsystem::GetInst(const UObject* _worldContextObject)
{
	if (UGameInstance* gameInst 
		= UGameplayStatics::GetGameInstance(_worldContextObject))
	{
		return gameInst->GetSubsystem<UQuestSubsystem>();
	}
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

// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"

#include "Catastrophe_VS.h"
#include "QuestSubsystem.h"
#include "QuestObjectiveComponent.h"

#include "Engine.h"

UQuest::UQuest()
{
	
}

void UQuest::SetQuestData(FQuestInfo _questInfo)
{
	QuestInfo = _questInfo;
}

void UQuest::AppendChildQuest(UQuest* _childQuest)
{
	ChildQuests.Add(_childQuest);
	_childQuest->AppendParentQuest(this);
}

void UQuest::AppendParentQuest(UQuest* _parentQuest)
{
	ParentQuests.Add(_parentQuest);
}

void UQuest::UnlockQuest()
{
	if (GEngine)
	{
		const FString msg = "Quest unlocked: " + QuestInfo.QuestName;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, msg);
	}
}

void UQuest::ActivateQuest()
{
	if (QuestObjectives.Num() > 0)
	{
		QuestObjectives[0]->ActivateObjective();
	}
	else
	{
		UE_LOG(LogQuestSystem, Warning, 
			TEXT("There is no objective in this quest: %s"), *(QuestInfo.QuestName));
	}
}

void UQuest::CompleteQuest()
{
	if (GEngine)
	{
		const FString msg = "Quest: " + QuestInfo.QuestName + " complete";
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, msg);
	}

	for (UQuest* quest : ChildQuests)
	{
		quest->UnlockQuest();
	}

}

void UQuest::CompleteObjective(class UQuestObjectiveComponent* _objective)
{
	// Check if it is the last objective or not
	int32 objectiveOrder = _objective->GetOrder();
	int32 nextObjectiveOrder = objectiveOrder + 1;
	if (nextObjectiveOrder < QuestObjectives.Num())
	{
		if (UQuestObjectiveComponent* nextObjective = QuestObjectives[nextObjectiveOrder])
		{
			nextObjective->ActivateObjective();
		}
		else
		{
			UE_LOG(LogQuestSystem, Error, TEXT("Objective order %s hasn't been assigned"));
		}
	}
	else
	{
		SetQuestState(EQuestState::Completed);
	}
}

void UQuest::SetQuestState(EQuestState _questState)
{
	QuestState = _questState;

	switch (QuestState)
	{
	case EQuestState::Locked:
		break;
	
	case EQuestState::Avaliable:
	{
		UnlockQuest();
	}
		break;
	
	case EQuestState::Active: 
	{
		ActivateQuest();
	}
		break;
	
	case EQuestState::Completed:
	{
		CompleteQuest();
	}
		break;

	default: break;
	}

}

void UQuest::RegisterObjective(class UQuestObjectiveComponent* _objective)
{
	QuestObjectives.Insert(_objective, _objective->GetOrder());
	_objective->SetOwningQuest(this);
}

UQuestSubsystem* UQuest::GetQuestSystem() const
{
	return Cast<UQuestSubsystem>(GetOuter());
}

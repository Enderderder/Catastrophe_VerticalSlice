// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"

#include "Catastrophe_VS.h"
#include "QuestSubsystem.h"
#include "QuestObjectiveComponent.h"

UQuest::UQuest()
{
	
}

void UQuest::SetQuestData(FQuestInfo _questInfo)
{
	QuestInfo = _questInfo;
}

void UQuest::SetQuestState(EQuestState _questState)
{
	QuestState = _questState;
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

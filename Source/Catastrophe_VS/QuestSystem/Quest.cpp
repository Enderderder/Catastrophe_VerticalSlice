// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"

#include "Catastrophe_VS.h"
#include "QuestSubsystem.h"

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

UQuestSubsystem* UQuest::GetQuestSystem() const
{
	return Cast<UQuestSubsystem>(GetOuter());
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CatastropheSaveGame.h"

#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "QuestSystem/Quest.h"
#include "QuestSystem/QuestSubsystem.h"

UCatastropheSaveGame::UCatastropheSaveGame()
	: USaveGame()
{
	SaveGameSlotName = TEXT("Slot0");
	UserIndex = 0;
}

void UCatastropheSaveGame::InitializeSaveGameInst(UGameInstance* _gameInst)
{
	UQuestSubsystem* questSystemInst = _gameInst->GetSubsystem<UQuestSubsystem>();
	if (questSystemInst)
	{
		int32 totalQuestCount = questSystemInst->GetTotalQuestCount();
		for (int32 index = 0; index < totalQuestCount; ++index)
		{
			SavedQuestState.Add(index, EQuestState::Locked);
		}
	}
}

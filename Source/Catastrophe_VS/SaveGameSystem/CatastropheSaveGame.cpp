// Fill out your copyright notice in the Description page of Project Settings.


#include "CatastropheSaveGame.h"

#include "QuestSystem/Quest.h"

UCatastropheSaveGame::UCatastropheSaveGame()
	: USaveGame()
{
	SaveGameSlotName = TEXT("Slot0");
	UserIndex = 0;
}

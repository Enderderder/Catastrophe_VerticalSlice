// Fill out your copyright notice in the Description page of Project Settings.


#include "CatastropheGameInstance.h"

#include "GameInstance/CatastropheGameInstanceSubsystem.h"

#include "QuestSystem/QuestSubsystem.h"
#include "SaveGameSystem/SaveGameSubsystem.h"

UCatastropheGameInstance::UCatastropheGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UCatastropheGameInstance::Init()
{
	Super::Init();
	
	/// Due to engine motherfucking uselesses, i cant do it automatically
	// Calles all the advance custom functions for all the 
	// UCatastropheGameInstanceSubsystem inside the subsystem collection
// 	const TArray<UCatastropheGameInstanceSubsystem*> catastropheSubsystems =
// 		GetSubsystemArray<UCatastropheGameInstanceSubsystem>();
// 	for (UCatastropheGameInstanceSubsystem* subSystem : catastropheSubsystems)
// 	{
// 		subSystem->PostInitialize();
// 	}

	// Manually load each custom subsystem into the array
	TArray<UCatastropheGameInstanceSubsystem*> catastropheSubsystems;
	catastropheSubsystems.Add(Cast<UCatastropheGameInstanceSubsystem>(
		GetSubsystem<USaveGameSubsystem>()));
	catastropheSubsystems.Add(Cast<UCatastropheGameInstanceSubsystem>(
		GetSubsystem<UQuestSubsystem>()));

	// Calles all the advance custom functions for all the 
	// UCatastropheGameInstanceSubsystem inside the subsystem collection
	for (UCatastropheGameInstanceSubsystem* subSystem : catastropheSubsystems)
 	{
 		subSystem->PostInitialize();
 	}

	




}

void UCatastropheGameInstance::Shutdown()
{
	Super::Shutdown();

	
}

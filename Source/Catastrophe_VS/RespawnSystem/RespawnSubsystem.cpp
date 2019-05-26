// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

URespawnSubsystem::URespawnSubsystem() 
	: UCatastropheGameInstanceSubsystem()
{}

void URespawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void URespawnSubsystem::PostInitialize()
{
	Super::PostInitialize();



}

void URespawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
}

URespawnSubsystem* URespawnSubsystem::GetInst(const UObject* _worldContextObject)
{
	if (UGameInstance * gameInst
		= UGameplayStatics::GetGameInstance(_worldContextObject))
	{
		return gameInst->GetSubsystem<URespawnSubsystem>();
	}
	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestObjectiveComponent.h"

#include "QuestSubsystem.h"

// Sets default values for this component's properties
UQuestObjectiveComponent::UQuestObjectiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestObjectiveComponent::BeginPlay()
{
	Super::BeginPlay();

	//UQuestSubsystem::GetInst(this)->Register
	
	
}


// Called every frame
void UQuestObjectiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestObjectiveComponent::ActivateObjective()
{


	//Broadcast the message
	OnObjectiveActivate.Broadcast();
}

void UQuestObjectiveComponent::CompleteObjective()
{


	// Boradcast the message
	OnObjectiveComplete.Broadcast();
}

void UQuestObjectiveComponent::FailObjective()
{

}

void UQuestObjectiveComponent::SetOwningQuest(class UQuest* _quest)
{
	OwningQuest = _quest;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestObjectiveComponent.h"

#include "QuestSubsystem.h"
#include "Quest.h"

// Sets default values for this component's properties
UQuestObjectiveComponent::UQuestObjectiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UQuestObjectiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register the objective into the quest system
	UQuestSubsystem::GetInst(this)->RegisterObjectiveToQuest(this, QuestID);
}

// Called every frame
void UQuestObjectiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestObjectiveComponent::ActivateObjective()
{
	// Set state
	ObjectiveState = EObjectiveState::Active;

	//Broadcast the message
	OnObjectiveActivate.Broadcast();
}

void UQuestObjectiveComponent::CompleteObjective()
{
	// Set state
	ObjectiveState = EObjectiveState::Completed;

	// Tell the quest that this objective is completed
	OwningQuest->CompleteObjective(this);

	// Boradcast the message
	OnObjectiveComplete.Broadcast();
}

void UQuestObjectiveComponent::FailObjective()
{
	// Set state
	ObjectiveState = EObjectiveState::Failed;

	// Broadcast the message
	OnObjectiveFailed.Broadcast();
}

void UQuestObjectiveComponent::SetOwningQuest(class UQuest* _quest)
{
	OwningQuest = _quest;
}


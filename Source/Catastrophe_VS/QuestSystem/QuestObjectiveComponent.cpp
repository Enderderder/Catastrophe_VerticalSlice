// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestObjectiveComponent.h"

#include "QuestSubsystem.h"
#include "Quest.h"

#include "Catastrophe_VS.h"

// Sets default values for this component's properties
UQuestObjectiveComponent::UQuestObjectiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	bAutoActivate = true;

}

// Called when the game starts
void UQuestObjectiveComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UQuestObjectiveComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Register the objective into the quest system
	if (UQuestSubsystem* questSystem = UQuestSubsystem::GetInst(this))
	{
		questSystem->RegisterObjectiveToQuest(this, QuestID);
	}
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

bool UQuestObjectiveComponent::IsObjectiveComplete()
{
	if (ObjectiveState == EObjectiveState::Completed)
	{
		return true;
	}
	return false;
}

bool UQuestObjectiveComponent::IsObjectiveActive()
{
	if (ObjectiveState == EObjectiveState::Active)
	{
		return true;
	}
	return false;
}

void UQuestObjectiveComponent::SetOwningQuest(class UQuest* _quest)
{
	OwningQuest = _quest;
}



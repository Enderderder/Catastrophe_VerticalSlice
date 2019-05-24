// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interactable/BaseClasses/InteractableComponent.h"
#include "QuestSystem/QuestObjectiveComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(root);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->RegisterTriggerVolume(TriggerBox);
	InteractableComponent->OnInteract.AddDynamic(this, &ANPC::Interact);
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	ConversationInProgress = false;
	CurrentDialogueNum = 0;
	CurrentQuest = 0;

	IsQuestStarted = false;
	CanNPCTalk = true;
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Interact(class APlayerCharacter* _playerCharacter)
{
	if (CanNPCTalk)
	{
		Receive_Interact();

		// Interaction functionality
		if (ConversationInProgress)
		{
			NextDialogue();
		}
		else
		{
			ConversationInProgress = true;
			InitializeWidget();
		}
	}
}

void ANPC::InitializeWidget()
{
	if (WidgetRef)
	{
		DialogueWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetRef);

		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport();
			UpdateWidget();
		}
	}
}

void ANPC::UpdateWidget()
{
	if (CurrentQuest < ConversationsList.Num())
	{
		if (!IsQuestStarted)
		{
			if (CurrentDialogueNum < ConversationsList[CurrentQuest].StartConversation.Num())
			{
				if (ConversationsList[CurrentQuest].StartConversation[CurrentDialogueNum].m_CharType == ECharacter::ECh_NPC)
				{
					CurrentNPCDialogueText = ConversationsList[CurrentQuest].StartConversation[CurrentDialogueNum].m_Sentence;
					CurrentPlayerDialogueText = "";
					IsNPCTalking = true;
				}
				else if (ConversationsList[CurrentQuest].StartConversation[CurrentDialogueNum].m_CharType == ECharacter::ECh_Player)
				{
					CurrentNPCDialogueText = "";
					CurrentPlayerDialogueText = ConversationsList[CurrentQuest].StartConversation[CurrentDialogueNum].m_Sentence;
					IsNPCTalking = false;
				}
			}
		}
		else
		{
			if (CurrentDialogueNum < ConversationsList[CurrentQuest].FinishedConversation.Num())
			{
				if (ConversationsList[CurrentQuest].FinishedConversation[CurrentDialogueNum].m_CharType == ECharacter::ECh_NPC)
				{
					CurrentNPCDialogueText = ConversationsList[CurrentQuest].FinishedConversation[CurrentDialogueNum].m_Sentence;
					CurrentPlayerDialogueText = "";
					IsNPCTalking = true;
				}
				else if (ConversationsList[CurrentQuest].FinishedConversation[CurrentDialogueNum].m_CharType == ECharacter::ECh_Player)
				{
					CurrentNPCDialogueText = "";
					CurrentPlayerDialogueText = ConversationsList[CurrentQuest].FinishedConversation[CurrentDialogueNum].m_Sentence;
					IsNPCTalking = false;
				}
			}
		}
	}

	Receive_UpdateWidget();
}

void ANPC::NextDialogue()
{
	if (CurrentQuest < ConversationsList.Num())
	{
		if (!IsQuestStarted)
		{
			// Increment current dialogue position by 1 if not max num of sentences
			if (CurrentDialogueNum < ConversationsList[CurrentQuest].StartConversation.Num() - 1)
			{
				CurrentDialogueNum++;
			}
			else // If at end of conversation
			{
				if (ConversationsList[CurrentQuest].FinishedConversation.Num() > 0)
				{
					IsQuestStarted = true;
				}
				FinishConversation();
			}
		}
		else
		{
			// Increment current dialogue position by 1 if not max num of sentences
			if (CurrentDialogueNum < ConversationsList[CurrentQuest].FinishedConversation.Num() - 1)
			{
				CurrentDialogueNum++;
			}
			else // If at end of conversation
			{
				IsQuestStarted = false;
				FinishConversation();
				CurrentQuest++;
				if (CurrentQuest >= ConversationsList.Num())
				{
					CanNPCTalk = false;
				}
			}
		}

		UpdateWidget();
	}
}

void ANPC::DisableDialogue()
{
	if (DialogueWidget != NULL)
	{
		DialogueWidget->RemoveFromViewport();
	}
	CurrentDialogueNum = 0;
	ConversationInProgress = false;
}

void ANPC::FinishConversation()
{
	DisableDialogue();
}

void ANPC::StartQuest()
{
	Receive_StartQuest();

	ConversationsList[CurrentQuest].Quest->ActivateObjective();
}

void ANPC::FinishQuest()
{
	Receive_FinishQuest();

	ConversationsList[CurrentQuest].Quest->CompleteObjective();
}

void ANPC::SetConversationQuest(int _index, class UQuestObjectiveComponent* _quest)
{
	ConversationsList[_index].Quest = _quest;
}

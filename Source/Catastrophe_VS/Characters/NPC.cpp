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

void ANPC::InitialInteract()
{
	Receive_InitialInteract();


}

void ANPC::Interact(class APlayerCharacter* _playerCharacter)
{
	if (CanNPCTalk)
	{
		Receive_Interact();

		// Interaction functionality
		if (ConversationInProgress)
		{
			/* Move to next line of dialogue in conversation */
			NextDialogue();
		}
		else
		{
			/* Initialize widget and start conversation */
			InitialInteract();
			ConversationInProgress = true;
			InitializeWidget();
		}
	}
}

void ANPC::InitializeWidget()
{
	/* Check if widget reference is not nullptr */
	if (WidgetRef)
	{
		/* Create widget */
		DialogueWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetRef);

		/* Add widget to screen and update values in widget */
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
			if (CurrentDialogueNum < ConversationsList[CurrentQuest].StartQuestConversation.Num())
			{
				if (ConversationsList[CurrentQuest].StartQuestConversation[CurrentDialogueNum].m_CharType == ECharacter::ECh_NPC)
				{
					CurrentNPCDialogueText = ConversationsList[CurrentQuest].StartQuestConversation[CurrentDialogueNum].m_Sentence;
					CurrentPlayerDialogueText = "";
					IsNPCTalking = true;
				}
				else if (ConversationsList[CurrentQuest].StartQuestConversation[CurrentDialogueNum].m_CharType == ECharacter::ECh_Player)
				{
					CurrentNPCDialogueText = "";
					CurrentPlayerDialogueText = ConversationsList[CurrentQuest].StartQuestConversation[CurrentDialogueNum].m_Sentence;
					IsNPCTalking = false;
				}
			}
		}
		else
		{
			if (CurrentDialogueNum < ConversationsList[CurrentQuest].FinishedQuestConversation.Num())
			{
				if (ConversationsList[CurrentQuest].FinishedQuestConversation[CurrentDialogueNum].m_CharType == ECharacter::ECh_NPC)
				{
					CurrentNPCDialogueText = ConversationsList[CurrentQuest].FinishedQuestConversation[CurrentDialogueNum].m_Sentence;
					CurrentPlayerDialogueText = "";
					IsNPCTalking = true;
				}
				else if (ConversationsList[CurrentQuest].FinishedQuestConversation[CurrentDialogueNum].m_CharType == ECharacter::ECh_Player)
				{
					CurrentNPCDialogueText = "";
					CurrentPlayerDialogueText = ConversationsList[CurrentQuest].FinishedQuestConversation[CurrentDialogueNum].m_Sentence;
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
			if (CurrentDialogueNum < ConversationsList[CurrentQuest].StartQuestConversation.Num() - 1)
			{
				CurrentDialogueNum++;
			}
			else // If at end of conversation
			{
				if (ConversationsList[CurrentQuest].FinishedQuestConversation.Num() > 0)
				{
					IsQuestStarted = true;
				}
				else
				{
					CanNPCTalk = false;
				}
				FinishOldQuest();
				FinishConversation();
			}

			StartNewQuest();
		}
		else
		{
			// Increment current dialogue position by 1 if not max num of sentences
			if (CurrentDialogueNum < ConversationsList[CurrentQuest].FinishedQuestConversation.Num() - 1)
			{
				CurrentDialogueNum++;
			}
			else // If at end of conversation
			{
				IsQuestStarted = false;
				FinishConversation();
				if (CurrentQuest >= ConversationsList.Num() - 1)
				{
					CanNPCTalk = false;
					FinishNewQuest();
					CurrentQuest++;
				}
			}
		}

		UpdateWidget();
	}
}

/* Removes the widget from the viewport */
void ANPC::DisableDialogue()
{
	Receive_DisableDialogue();
	if (DialogueWidget != NULL)
	{
		DialogueWidget->RemoveFromViewport();
	}

	/* End conversation and reset variables */
	CurrentDialogueNum = 0;
	ConversationInProgress = false;
}

void ANPC::FinishConversation()
{
	DisableDialogue();
}

void ANPC::StartNewQuest()
{
	Receive_StartNewQuest();
}

void ANPC::FinishOldQuest()
{
	Receive_FinishQuests();
	Receive_FinishOldQuest();

	if (ConversationsList[CurrentQuest].bFinishOldQuest)
	{
		ConversationsList[CurrentQuest].OldQuest->CompleteObjective();
	}
	else
	{
		// Start StartQuest
	}
}

void ANPC::FinishNewQuest()
{
	Receive_FinishQuests();
	Receive_FinishNewQuest();

	if (ConversationsList[CurrentQuest].bFinishNewQuest)
	{
		ConversationsList[CurrentQuest].NewQuest->CompleteObjective();
	}
	else
	{
		// Start EndQuest
	}
}

void ANPC::SetConversationQuests(int _index, class UQuestObjectiveComponent* _OldQuest, class UQuestObjectiveComponent* _NewQuest)
{
	ConversationsList[_index].OldQuest = _OldQuest;
	ConversationsList[_index].NewQuest = _NewQuest;
}

int ANPC::GetCurrentFishboneReward_OldQuest()
{
	if (CurrentQuest < ConversationsList.Num())
	{
		return (ConversationsList[CurrentQuest].FishBonesReward_OldQuest);
	}
	return 0;
}

int ANPC::GetCurrentFishboneReward_NewQuest()
{
	if (CurrentQuest < ConversationsList.Num())
	{
		return (ConversationsList[CurrentQuest].FishBonesReward_NewQuest);
	}
	return 0;
}

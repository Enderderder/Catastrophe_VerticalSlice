// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interactable/BaseClasses/InteractableComponent.h"
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
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	InteractableComponent->RegisterTriggerVolume(TriggerBox);
	InteractableComponent->OnInteract.AddDynamic(this, &ANPC::Interact);

	ConversationInProgress = false;
	CurrentDialogueNum = 0;
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Interact(class APlayerCharacter* _playerCharacter)
{
	Receive_Interact();

	// Interaction functionality
	if (ConversationInProgress)
	{
		NextDialogue();
	}
	else
	{
		InitializeWidget();
		ConversationInProgress = true;
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
	if (DialogueSentenceList[CurrentDialogueNum].m_CharType == ECharacter::ECh_NPC)
	{
		// Set character image on dialogue widget to NPC image
		CurrentIcon = NPCIcon;
	}
	else if (DialogueSentenceList[CurrentDialogueNum].m_CharType == ECharacter::ECh_Player)
	{
		// Set character image on dialogue widget to Player image
		CurrentIcon = PlayerIcon;
	}

	CurrentDialogueText = DialogueSentenceList[CurrentDialogueNum].m_Sentence;

	Receive_UpdateWidget();
}

void ANPC::NextDialogue()
{
	// Increment current dialogue position by 1 if not max num of sentences
	if (CurrentDialogueNum < DialogueSentenceList.Num() - 1)
	{
		CurrentDialogueNum++;
	}
	else
	{
		DisableDialogue();
	}

	UpdateWidget();
}

void ANPC::DisableDialogue()
{
	DialogueWidget->RemoveFromViewport();
	CurrentDialogueNum = 0;
	ConversationInProgress = false;
}

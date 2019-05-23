// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

UENUM(BlueprintType)
enum class ECharacter : uint8
{
	ECh_NPC		UMETA(DisplayName = "NPC"),
	ECh_Player	UMETA(DisplayName = "Player")
};

USTRUCT(BlueprintType)
struct FSSentence
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ECharacter m_CharType;

	UPROPERTY(EditAnywhere)
	FString m_Sentence;
};

USTRUCT(BlueprintType)
struct FConversation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSentence> StartConversation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSSentence> FinishedConversation;

	UPROPERTY(BlueprintReadWrite)
	class UQuestObjectiveComponent* Quest;
};

UCLASS()
class CATASTROPHE_VS_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* DialogueWidget;

	// Dialogue settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue: NPC")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue: NPC")
	class UTexture* NPCIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue: Text")
	TSubclassOf<class UUserWidget> WidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue: Text")
	TArray<FConversation> ConversationsList;

	UPROPERTY()
	int CurrentDialogueNum;

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue: Text")
	FString CurrentPlayerDialogueText;

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue: Text")
	FString CurrentNPCDialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue: Interaction")
	bool CanGiveQuests;

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue: Interaction")
	bool ConversationInProgress;

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue: Interaction")
	bool IsNPCTalking;

	UPROPERTY()
	bool IsQuestStarted;

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue: Interaction")
	int CurrentQuest;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void Interact(class APlayerCharacter* _playerCharacter);
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC", meta = (DisplayName = "OnInteract"))
	void Receive_Interact();

	UFUNCTION()
	void InitializeWidget();

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void UpdateWidget();
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC", meta = (DisplayName = "OnUpdateWidget"))
	void Receive_UpdateWidget();

	UFUNCTION()
	void NextDialogue();

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void DisableDialogue();

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void FinishConversation();

	UFUNCTION(BlueprintCallable, Category = "NPC: Quests")
	void StartQuest();
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC: Quests", meta = (DisplayName = "OnQuestStart"))
	void Receive_StartQuest();

	UFUNCTION(BlueprintCallable, Category = "NPC: Quests")
	void FinishQuest();
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC: Quests", meta = (DisplayName = "OnQuestFinish"))
	void Receive_FinishQuest();

	UFUNCTION(BlueprintCallable, Category = "NPC: Conversation")
	void SetConversationQuest(int _index, class UQuestObjectiveComponent* _quest);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

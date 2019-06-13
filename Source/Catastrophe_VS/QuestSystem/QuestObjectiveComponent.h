// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestObjectiveComponent.generated.h"

/**  */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveActivateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveCompleteSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveFailedSignature);

/**
 * State of the quest objectives
 */
UENUM(BlueprintType)
enum class EObjectiveState : uint8
{
	Inactive = 0,
	Active,
	Completed,
	Failed,
};

/**
 * This component is for customize objectives
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_VS_API UQuestObjectiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestObjectiveComponent();

	/** Called when a quest activate the objective */
	UPROPERTY(BlueprintAssignable)
	FObjectiveActivateSignature OnObjectiveActivate;

	/** Called when the player complete the objective */
	UPROPERTY(BlueprintAssignable)
	FObjectiveCompleteSignature OnObjectiveComplete;

	/** Called when the player fail the objective */
	UPROPERTY(BlueprintAssignable)
	FObjectiveFailedSignature OnObjectiveFailed;

protected:

	/** The ID of the quest that this objective is belong to */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystem")
	int32 QuestID = 0;

	/** The pointer to the quest that this objective is belong to */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystem")
	class UQuest* OwningQuest;

	/** The order of the objective in the quest */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystem")
	int32 ObjectiveOrder = 0;

	/** A short description of the objective */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystem")
	FString ShortDescription = "Default Description";

	/** The state of the objective */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EObjectiveState ObjectiveState = EObjectiveState::Inactive;

	/** Will this objective complete upon activate? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSystem")
	bool bAutoComplete = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/** Called when the component is initialized */
	virtual void InitializeComponent() override;

	/** Objective action | Activates the objective */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void ActivateObjective();

	/** Objective action | Completes the objective */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void CompleteObjective();

	/** Objective action | Failes the objective */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void FailObjective();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	bool IsObjectiveComplete() const;

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	bool IsObjectiveActive() const;

	/** Sets the quest owner */
	void SetOwningQuest(class UQuest* _quest);

	/** Getter */
	FORCEINLINE int32 GetOrder() const { return ObjectiveOrder; }
	FORCEINLINE FString GetDescription() const { return ShortDescription; }
	FORCEINLINE class UQuest* GetQuestOwner() const { return OwningQuest; }
	/** Getter End */
};

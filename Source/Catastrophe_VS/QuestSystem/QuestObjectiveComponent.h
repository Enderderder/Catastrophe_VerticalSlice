// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestObjectiveComponent.generated.h"

/**  */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveActivateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveCompleteSignature);





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
	FObjectiveActivateSignature OnObjectiveActivate;

	/** Called when the player complete the objective */
	FObjectiveCompleteSignature OnObjectiveComplete;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystem")
	int32 QuestID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystem")
	class UQuest* OwningQuest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystem")
	int32 ObjectiveOrder = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystem")
	FString ShortDescription = "Default Description";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSystem")
	bool bAutoComplete = false;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void ActivateObjective();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void CompleteObjective();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void FailObjective();



	/** Sets the quest owner */
	void SetOwningQuest(class UQuest* _quest);

	/** Getter */
	FORCEINLINE int32 GetOrder() const { return ObjectiveOrder; }
	FORCEINLINE FString GetDescription() const { return ShortDescription; }
	FORCEINLINE class UQuest* GetQuestOwner() const { return OwningQuest; }
	/** Getter End */

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/BaseClasses/SimpleInteractableAnimated.h"
#include "StackedCrates.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API AStackedCrates : public ASimpleInteractableAnimated
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BlockVolume;
	
public:
	// Sets default values for this actor's properties
	AStackedCrates();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "PlayCrateAnim"))
	void Receive_PlayCrateAnim();

public:

	/** Interactable Object Interface */
	virtual void OnInteract_Implementation(class APlayerCharacter* _actor) override;
	/** Interface End */
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/BaseClasses/SimpleInteractableStatic.h"
#include "TomatoBox.generated.h"

/**
 * This is an interactable object that allow player grab a tomato in his hand
 */
UCLASS()
class CATASTROPHE_VS_API ATomatoBox : public ASimpleInteractableStatic
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	ATomatoBox();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
		class APlayerCharacter* PlayerReference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "PickUpTomatoCheck"))
	void PickUpTomato();

public:
	/* Interactable Object Interface */
	virtual void OnInteract_Implementation(class APlayerCharacter* _actor) override;
	/* Interface End */
};

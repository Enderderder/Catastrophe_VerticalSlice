// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CATASTROPHE_VS_API IInteractableObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(class APlayerCharacter* _actor);
	virtual void OnInteract_Implementation(class APlayerCharacter* _actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnUnInteract(class APlayerCharacter* _actor);
	virtual void OnUnInteract_Implementation(class APlayerCharacter* _actor);
};

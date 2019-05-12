// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "InteractableObject.h"
#include "SimpleInteractableStatic.generated.h"

/**
 * This is a base class of an interactable object
 * that constant a static mesh component and a box component
 */
UCLASS()
class CATASTROPHE_VS_API ASimpleInteractableStatic : public AStaticMeshActor, public IInteractableObject
{
	GENERATED_BODY()

private:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UBoxComponent* InteractionTrigger;

public:
	// Sets default values for this actor's properties
	ASimpleInteractableStatic();

	/** Getter */
	//FORCEINLINE class UBoxComponent* GetTriggerBox() { return InteractionTrigger; }
	/** Getter End */
};

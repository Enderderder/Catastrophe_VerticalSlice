// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "InteractableObject.h"
#include "SimpleInteractableAnimated.generated.h"

/**
 * This is a base class of an interactable object 
 * that contant a skeletal mesh component and a box component
 */
UCLASS()
class CATASTROPHE_VS_API ASimpleInteractableAnimated : public ASkeletalMeshActor, public IInteractableObject
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractionTrigger;

public:
	// Sets default values for this actor's properties
	ASimpleInteractableAnimated();

protected:
	virtual void BeginPlay() override;

	// Trigger component overlap functinon
	UFUNCTION()
	virtual void OnInteractionTriggerOverlapBegin(class UPrimitiveComponent* _overlappedComponent, class AActor* _otherActor, class UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION()
	virtual void OnInteractionTriggerOverlapEnd(class UPrimitiveComponent* _overlappedComponent, class AActor* _otherActor, class UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);


public:

	/** Getter */
	FORCEINLINE class UBoxComponent* GetTriggerBox() { return InteractionTrigger; }
	/** Getter End */

};

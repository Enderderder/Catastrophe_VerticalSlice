// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

// Delegate that has one parameter
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractSingature, class APlayerCharacter*, _playerCharacter);

/**
 * This component control and sends out signal when player is interacting
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_VS_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	/** Even called when the player interact with this component */
	UPROPERTY(BlueprintAssignable)
	FInteractSingature OnInteract;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	bool bAutoInteract = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 TriggerCounter = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class APlayerCharacter* PlayerRef;

protected:

	/** Called when registered component overlap event triggers */
	UFUNCTION()
	void OnTriggerWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when registered component end overlap event triggers*/
	UFUNCTION()
	void OnTriggerEndWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Called when player interact */
	void Interact(class APlayerCharacter* _playerCharacter);

	/** Register a component that has some trigger volume */
	void RegisterTriggerVolume(class UPrimitiveComponent* _component);

	
};

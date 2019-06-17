// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

// Delegates
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	float HoldingTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float RequiredHoldTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	bool bOneTimeUse = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float UIShowingDistance = 500.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interaction")
	bool bShowingUI = false;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	bool bAutoInteract = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 TriggerCounter = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class APlayerCharacter* PlayerRef;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	class USceneComponent* InteractableUI;

private:

	UPROPERTY()
	bool bWantToShowUI = false;

protected:

	/** Called when registered component overlap event triggers */
	UFUNCTION()
	void OnTriggerWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when registered component end overlap event triggers*/
	UFUNCTION()
	void OnTriggerEndWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	/** Called each frame */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Called when player interact */
	void Interact(class APlayerCharacter* _playerCharacter, float _holdTime);

	/** Called when the player hold the interaction */
	void InteractHold(class APlayerCharacter* _playerCharacter, float _holdTime);

	/**
	 * Register a component that has some trigger volume
	 * @note This function is prefer to be called in constructor
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RegisterTriggerVolume(class UPrimitiveComponent* _component);

	/**
	 * Register a ui component to be shown while player is able to interact with
	 * @note This function is prefer to be called in constructor
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RegisterUiComponent(class USceneComponent* _uiComponent);

};

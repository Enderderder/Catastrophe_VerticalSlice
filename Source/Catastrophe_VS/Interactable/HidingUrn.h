// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/BaseClasses/SimpleInteractableStatic.h"
#include "HidingUrn.generated.h"

/**
 * This is an interactable object that allows the player to hide in urn to avoid detection
 */
UCLASS()
class CATASTROPHE_VS_API AHidingUrn : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AHidingUrn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	class APlayerCharacter* PlayerReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HideInUrn")
	bool bHasUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HideInUrn")
	bool bPlayerIn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Hide in urn logic which is called when player interacts
	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "HideInUrn"))
	void HideInUrn();

	UFUNCTION(BlueprintCallable)
	void JumpIn();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HideInUrn", meta = (DisplayName = "OnJumpIn"))
	void Receive_JumpIn();

	UFUNCTION(BlueprintCallable)
	void JumpOut();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HideInUrn", meta = (DisplayName = "OnJumpOut"))
	void Receive_JumpOut();
public:
	// Called every frame
	virtual void Tick(float DeltaTime);

	UFUNCTION()
		void OnPlayerInteract(class APlayerCharacter* _playerCharacter);

	/* Interactable Object Interface */
	//virtual void OnInteract_Implementation(class APlayerCharacter* _actor) override;
	/* Interface End */
};

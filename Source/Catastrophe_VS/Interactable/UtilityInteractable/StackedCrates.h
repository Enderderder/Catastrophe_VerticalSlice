// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StackedCrates.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API AStackedCrates : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* CratesMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BlockVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;
	
public:
	// Sets default values for this actor's properties
	AStackedCrates();

protected:
	virtual void BeginPlay() override;

	/** Called on player interact action */
	UFUNCTION()
	void OnPlayerInteract(class APlayerCharacter* _playerCharacter);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "PlayCrateAnim"))
	void Receive_PlayCrateAnim();

public:



};

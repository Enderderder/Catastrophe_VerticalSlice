// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HidingUrn.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTempPlayerInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerMaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerLocation;

	FTempPlayerInfo() :
		PlayerMaxWalkSpeed(0.0f),
		PlayerLocation(0.0f, 0.0f, 0.0f)
	{}
};

/**
 * This is an interactable object that allows the player to hide in urn to avoid detection
 */
UCLASS()
class CATASTROPHE_VS_API AHidingUrn : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDestructibleComponent* UrnDestructableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* UrnOutline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BlockVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

public:
	// Sets default values for this actor's properties
	AHidingUrn();

protected:

	/** Maximum time that the player can hide in this urn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Urn")
	float MaxHideTime = 10.0f;

	/** Minimum time that the player needs to be in the urn before allow to get out */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Urn")
	float MinHideTime = 1.0f;

	/** Determine the player is in the urn or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Urn")
	bool bPlayerIn = false;

	/** If the player is allowed to be out manually */
	bool bAllowManualJumpOut = false;

	/** Delegate the called when player jumps out on timer */
	FTimerDelegate JumpOutTimerDel;

	/** Timer handle for the maximum hiding time */
	UPROPERTY(BlueprintReadOnly, Category = "Urn")
	FTimerHandle UrnMaxHideTimerHandle;

	/** Timer handle for the minmum hiding time */
	UPROPERTY(BlueprintReadOnly, Category = "Urn")
	FTimerHandle UrnMinHideTimerHandle;

	/** Store some temporary informations when jumping in */
	UPROPERTY(BlueprintReadOnly, Category = "Urn")
	FTempPlayerInfo TempPlayerInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the player jumps into the urn */
	UFUNCTION()
	void JumpIn(class APlayerCharacter* _playerCharacter);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HideInUrn", meta = (DisplayName = "OnJumpIn"))
	void Receive_JumpIn(class APlayerCharacter* _playerCharacter);

	/** Called when the player jumps out of the urn */
	UFUNCTION()
	void JumpOut(class APlayerCharacter* _playerCharacter);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HideInUrn", meta = (DisplayName = "OnJumpOut"))
	void Receive_JumpOut(class APlayerCharacter* _playerCharacter);

	/** Called to allow player manually jumps out */
	void AllowManualJumpOut();

public:
	/** Called every tick */
	virtual void Tick(float DeltaTime) override;

	/** Called when the actor has destroyed */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Binded function calls on player interaction */
	UFUNCTION()
	void OnPlayerInteract(class APlayerCharacter* _playerCharacter);

};

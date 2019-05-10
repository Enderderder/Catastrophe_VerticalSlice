// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbableStall.generated.h"

/**
 * This is an interactable object that allow player climb up to the top of the stall with precision
 */
UCLASS()
class CATASTROPHE_VS_API AClimbableStall : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	class APlayerCharacter* PlayerReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(Category = "Interaction", VisibleAnywhere, BlueprintReadOnly)
	TArray<class USceneComponent*> WayPointArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

public:
	// Sets default values for this actor's properties
	AClimbableStall();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float TotalTransferTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	float TotalDistanceTravel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bHasUsed;

	/** Determent if the player has interact with this or not  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bInUse;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bUsable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bInWayPointTransit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	FVector CurrentWayPointLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	FVector DestinationLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	int WayPointCount;

	UPROPERTY()
	bool bHasSetPath;

	UPROPERTY()
	float CurrentPathLength;

	UPROPERTY()
	float LerpDelta;

	UPROPERTY()
	float TimeForCurrentPath;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Finds out the total distance needs to travel as soon as the player interact */
	UFUNCTION()
	float FindTotalDistance();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void InteractionStarting(class APlayerCharacter* _playerCharacter);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta = (DisplayName = "OnInteractionStart"))
	void Recieve_InteractionStart();
	
	UFUNCTION()
	void InteractionEnd();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta = (DisplayName = "OnInteractionEnd"))
	void Recieve_InteractionEnd();
};

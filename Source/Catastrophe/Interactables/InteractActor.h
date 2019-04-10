// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractActor.generated.h"

/**
 * Interact Actor are actors that have a mesh, a trigger box, also have implementable function for player to interact with.
 * They are responsible for open up the interact ability to the player as the player triggers within certain range.
 */
UCLASS()
class CATASTROPHE_API AInteractActor : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class APlayerCharacter* PlayerReference;

	// Store the info if this interaction actor is useable by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bUseable;

public:	
	// Sets default values for this actor's properties
	AInteractActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called by OnTriggerEnter()
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnTriggerEnterCheck(class AActor* _otherActor);
	
	// Called by OnTriggerExit()
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnTriggerrExitCheck(class AActor* _otherActor);

protected:

	// Called as the player triggers and able to use the interaction ability(s)
	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called as the player leave the trigger and no longer able to use the interaction ability(s)
	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called as the player is interacting with this actor
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "OnInteractStart"))
	void ReciveInteractionStart();

	// Called as the player is interacting with this actor
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "OnInteractEnd"))
	void ReciveInteractionEnd();

	// Called as the player is interacting with this actor (C++)
	virtual void OnInteractionStart();

	// Called as the player is interacting with this actor (C++)
	virtual void OnInteractionEnd();

	// Called when the actor interaction should begin
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void BeginInteraction();

	// Called when the actor interaction should end
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void EndInteraction();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DisableInteractActor();

	UFUNCTION(BlueprintCallable)
	class APlayerCharacter* GetPlayerRef() { return PlayerReference; }
};

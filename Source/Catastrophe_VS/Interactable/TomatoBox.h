// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractActor.h"
#include "TomatoBox.generated.h"

/**
 * This is an interactable object that allow player grab a tomato in his hand
 */
UCLASS()
class CATASTROPHE_VS_API ATomatoBox : public AInteractActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

public:
	// Sets default values for this actor's properties
	ATomatoBox();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called as the player is interacting with this actor (C++)
	virtual void OnInteractionStart() override;

	// Called as the player is interacting with this actor (C++)
	virtual void OnInteractionEnd() override;



};

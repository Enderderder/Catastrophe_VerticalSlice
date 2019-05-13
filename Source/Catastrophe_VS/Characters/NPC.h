// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

UCLASS()
class CATASTROPHE_VS_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString Name;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Interact(class APlayerCharacter* _playerCharacter);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "NPC", meta = (DisplayName = "OnInteract"))
	void Receive_Interact();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

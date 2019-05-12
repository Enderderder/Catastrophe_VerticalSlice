// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureChest.generated.h"

UCLASS()
class CATASTROPHE_VS_API ATreasureChest : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ChestMesh;

	// Later on will be adding animation to the chest
	///UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	///class USkeletalMeshComponent* ChestMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableCompoenent;
	
public:	
	// Sets default values for this actor's properties
	ATreasureChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called on player interact */
	UFUNCTION()
	void OnPlayerInteract(class APlayerCharacter* _playerCharacter);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() { return ChestMesh; }
	///FORCEINLINE class USkeletalMeshComponent* GetMesh() { return ChestMesh; }
	/** Getter End */

};

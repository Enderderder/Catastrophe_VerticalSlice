// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TomatoSack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_VS_API UTomatoSack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTomatoSack();

protected:
	
	UPROPERTY(EditDefaultsOnly)
	uint8 TomatoAmount;

	UPROPERTY(EditDefaultsOnly)
	uint8 MaxTomatoAmount;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	/** Addes one tomato into the sack, does not over flow */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void AddTomato();

	/** Addes certain amount of tomato into the sack, does not overflow */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void AddTomatoes(uint8 _Amount);

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void FillTomatoSack();

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	bool IsTomatoSackFull() const;

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void RemoveTomato();

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void RemoveTomatoes(uint8 _Amount);

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void EmptyTomatoSack();

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	bool IsTomatoSackEmpty() const;

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void SetTomatoAmount(uint8 _Amount);

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	uint8 GetTomatoAmount() const;

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void SetSackSize(uint8 _Size);

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	uint8 GetSackSize() const;

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	bool IsAbleToThrow();
};

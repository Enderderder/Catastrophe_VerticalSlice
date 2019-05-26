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

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void AddTomato();
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void AddTomatoes(uint8 _Amount);

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void FillTomatoSack();
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	bool IsTomatoSackFull();

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void RemoveTomato();
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void RemoveTomatoes(uint8 _Amount);

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void EmptyTomatoSack();
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	bool IsTomatoSackEmpty();

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void SetTomatoAmount(uint8 _Amount);
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	uint8 GetTomatoAmount();

	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void SetSackSize(uint8 _Size);
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	uint8 GetSackSize();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

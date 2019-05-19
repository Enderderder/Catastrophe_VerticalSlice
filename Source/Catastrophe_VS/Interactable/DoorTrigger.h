// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorTrigger.generated.h"

UCLASS()
class CATASTROPHE_VS_API ADoorTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorTrigger();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ActivateDoor();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DoorTrigger", meta = (DisplayName = "OnDoorActivation"))
	void Receive_ActivateDoorTrigger();

	UFUNCTION()
	void SetDoorActive(bool _b);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DoorTrigger", meta = (DisplayName = "OnSetDoorActive"))
	void Recieve_SetDoorActive(bool _b);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

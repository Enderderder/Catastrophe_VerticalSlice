// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

UCLASS()
class CATASTROPHE_VS_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* ShopWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void WidgetInit(TSubclassOf<class UUserWidget> _WidgetRef);

	UFUNCTION(BlueprintCallable)
	void Interact(class APlayerCharacter* _playerCharacter);
	UFUNCTION(BlueprintImplementableEvent, Category = "Shop", meta = (DisplayName = "OnInteract"))
	void Receive_Interact();
	
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void EnableShop();
	UFUNCTION(BlueprintImplementableEvent, Category = "Shop", meta = (DisplayName = "OnEnableShop"))
	void Receive_EnableShop();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void DisableShop();
	UFUNCTION(BlueprintImplementableEvent, Category = "Shop", meta = (DisplayName = "OnDisableShop"))
	void Receive_DisableShop();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

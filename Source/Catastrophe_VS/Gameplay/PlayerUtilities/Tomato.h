// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tomato.generated.h"

UCLASS()
class CATASTROPHE_VS_API ATomato : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TomatoMesh;

public:	
	// Sets default values for this actor's properties
	ATomato();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the mesh component overlapped */
	UFUNCTION()
	void OnTomatoOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called to destroy the tomato */
	UFUNCTION()
	void DestroyTomato(class AActor* _otherActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Tomato")
	void Receive_OnTomatoSplash(class AActor* _otherActor);

public:	

	/** Lauch tomato in certain direction */
	UFUNCTION(BlueprintCallable, Category = "Tomato")
	void LaunchTomato(FVector _launchDirection, float _launchForce);

	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return TomatoMesh; }
	/** Getter End */

};

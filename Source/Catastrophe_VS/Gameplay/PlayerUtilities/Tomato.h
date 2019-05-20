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
	class UStaticMeshComponent* Mesh;

public:	
	// Sets default values for this actor's properties
	ATomato();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the mesh component overlapped */
	UFUNCTION()
	void OnTomatoOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	

	/** TODO: Lauch tomato in certain direction */
	UFUNCTION(BlueprintCallable, Category = "Tomato")
	void LaunchTomato();

	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return Mesh; }
	/** Getter End */

};

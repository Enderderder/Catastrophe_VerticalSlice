// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnSubsystem.h"
#include "RespawnPoint.generated.h"


UCLASS()
class CATASTROPHE_VS_API ARespawnPoint : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* EditorOnlyMesh;
	
public:	
	// Sets default values for this actor's properties
	ARespawnPoint();

protected:

	/** The district this spawn point should belong to */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "RespawnSystem")
	EDISTRICT District;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;

};

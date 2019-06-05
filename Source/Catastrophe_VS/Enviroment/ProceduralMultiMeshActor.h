// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMultiMeshActor.generated.h"

/**
 * A struct that contains mesh infomation
 */
USTRUCT(BlueprintType)
struct FRandomMeshSlot
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MeshSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class UStaticMesh*> RandomMeshes;

	FRandomMeshSlot() :
		MeshSlotName("DefaultName")
	{}
};

/**
 * This is a procedually generated set of mesh
 */
UCLASS()
class CATASTROPHE_VS_API AProceduralMultiMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMultiMeshActor();

	/** Set if this asset will be regenerate each time it gets spawned into the world */
	UPROPERTY(EditAnywhere, Category = "ProceduralGenerateMesh")
	bool bRegenerateAtSpawn = true;

protected:

	/** Note: This is not for run time, this is use like a button by PostPropertyChange */
	UPROPERTY(EditAnywhere, Category = "ProceduralGenerateMesh")
	bool bGenerate = false;

	/** Sets the mesh for diff mesh component */
	UPROPERTY(EditDefaultsOnly, Category = "ProceduralGenerateMesh")
	TArray<FRandomMeshSlot> RandomMeshes;

	/** Array that stores all the mesh components */
	UPROPERTY(EditDefaultsOnly, Category = "ProceduralGenerateMesh")
	TArray<class UStaticMeshComponent*> StoredMeshComponents;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Called when construction happen */
	virtual void OnConstruction(const FTransform& Transform) override;


#if WITH_EDITOR
	/** Called when property changed */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:

	/** Called to refresh the meshes */
	void ReloadMeshes();

	/** Called to find static mesh component by name */
	class UStaticMeshComponent* GetStaticMeshByName(FName _name);


	///** Called to create new component */
	//class UStaticMeshComponent* CreateNewMeshComponent(FName _name);


};

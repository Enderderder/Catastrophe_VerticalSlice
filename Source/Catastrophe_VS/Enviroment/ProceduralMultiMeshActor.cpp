// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMultiMeshActor.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AProceduralMultiMeshActor::AProceduralMultiMeshActor()
{
 	// I don't need it :C
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AProceduralMultiMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
}

#if WITH_EDITOR
void AProceduralMultiMeshActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Get the property name
	FName propertyName = PropertyChangedEvent.GetPropertyName();

	// If generated checked
	if (propertyName == GET_MEMBER_NAME_CHECKED(AProceduralMultiMeshActor, bGenerate))
	{
		if (bGenerate)
		{
			ReloadMeshes();

			// Reset
			bGenerate = false;
		}
	}

	// Call the parent class method
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AProceduralMultiMeshActor::ReloadMeshes()
{
// 	if (StoredMeshComponents.Num() != MeshCount)
// 	{
// 		if (StoredMeshComponents.Num() > MeshCount)
// 		{
// 
// 		}
// 	}

// 	TArray<UStaticMeshComponent*> newCompArray;
// 	newCompArray.Reserve(RandomMeshes.Num());
// 
// 	for (int32 index = 0; 
// 		index < newCompArray.Num() || index < StoredMeshComponents.Num(); ++index)
// 	{
// 		//if (index >= )
// 	}

}

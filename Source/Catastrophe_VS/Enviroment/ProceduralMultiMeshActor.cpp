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
	
	// Reload at beginplay
	ReloadMeshes();

}



//class UStaticMeshComponent* AProceduralMultiMeshActor::CreateNewMeshComponent(FName _name)
//{
//	UStaticMeshComponent* newComp =
//		NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), _name);
//	if (!newComp)
//	{
//		return nullptr;
//	}
//	
//	//newComp->RegisterComponent();
//	newComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
//	return newComp;
//}

void AProceduralMultiMeshActor::OnConstruction(const FTransform& Transform)
{
 	Super::OnConstruction(Transform);
// 	
// 	// Get all the static mesh components
// 	TInlineComponentArray<UStaticMeshComponent*> meshComponents;
// 	GetComponents<UStaticMeshComponent>(meshComponents);
// 	StoredMeshComponents = meshComponents;
// 
// 	for (int32 index = 0; 
// 		index < StoredMeshComponents.Num() && index < RandomMeshes.Num(); ++index)
// 	{
// 		int32 randomMeshIndex =
// 			FMath::RandRange(0, RandomMeshes[index].RandomMeshes.Num() - 1);
// 		StoredMeshComponents[index]->SetStaticMesh(
// 			RandomMeshes[index].RandomMeshes[randomMeshIndex]);
// 	}
}

void AProceduralMultiMeshActor::ReloadMeshes()
{
	// 	TArray<UStaticMeshComponent*> newCompArray;
	// 
	// 	for (int32 index = 0; 
	// 		index < RandomMeshes.Num() || index < StoredMeshComponents.Num(); ++index)
	// 	{
	// 		// If old component runs out first, add the rest as new component
	// 		if (index >= StoredMeshComponents.Num())
	// 		{
	// 			for (index; index < RandomMeshes.Num(); ++index)
	// 			{
	// 				UStaticMeshComponent* newMeshComp = 
	// 					CreateNewMeshComponent(RandomMeshes[index].MeshSlotName);
	// 				newCompArray.Add(newMeshComp);
	// 			}
	// 			break;
	// 		}
	// 		// If there is more, delete the rest
	// 		else if (index >= newCompArray.Num())
	// 		{
	// 			for (index; index < newCompArray.Num(); ++index)
	// 			{
	// 				StoredMeshComponents[index]->DestroyComponent();
	// 			}
	// 			break;
	// 		}
	// 
	// 		// Move the component from old array to the new one and rename them
	// 		newCompArray.Add(StoredMeshComponents[index]);
	// 		StoredMeshComponents[index]->Rename(
	// 			*(RandomMeshes[index].MeshSlotName.ToString()), this);
	// 	}
	// 
	// 	// Swap out the array at the end
	// 	StoredMeshComponents = newCompArray;



	// Check if there is any random meshes
	if (RandomMeshes.Num() <= 0)
	{
		return;
	}

	// Get all the static mesh components
	TInlineComponentArray<UStaticMeshComponent*> meshComponents;
	GetComponents<UStaticMeshComponent>(meshComponents);
	StoredMeshComponents = meshComponents;

	for (int32 i = 0; i < RandomMeshes.Num(); ++i)
	{
		if (UStaticMeshComponent* staticMeshComponent = GetStaticMeshByName(RandomMeshes[i].MeshSlotName))
		{
			int32 randomMeshIndex =
				FMath::RandRange(0, RandomMeshes[i].RandomMeshes.Num() - 1);
			staticMeshComponent->SetStaticMesh(RandomMeshes[i].RandomMeshes[randomMeshIndex]);
		}
		else return;
	}
}

UStaticMeshComponent* AProceduralMultiMeshActor::GetStaticMeshByName(FName _name) 
{
	UStaticMeshComponent* resultComponent = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(_name));

	if (!resultComponent)
		UE_LOG(LogTemp, Warning, TEXT("Procedural Mesh Generator: Cannot find the mesh component with name"));

	return resultComponent;
}

#if WITH_EDITOR
void AProceduralMultiMeshActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName propertyName = PropertyChangedEvent.GetPropertyName();

	if (propertyName == GET_MEMBER_NAME_CHECKED(AProceduralMultiMeshActor, bGenerate))
	{
		if (bGenerate)
		{
			ReloadMeshes();

			bGenerate = false;
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
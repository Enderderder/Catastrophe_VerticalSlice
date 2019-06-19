// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnPoint.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ARespawnPoint::ARespawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	EditorOnlyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EditorOnlyMesh"));
	//EditorOnlyMesh->bIsEditorOnly = true;
	EditorOnlyMesh->bTickInEditor = false;
	EditorOnlyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EditorOnlyMesh->SetGenerateOverlapEvents(false);
	EditorOnlyMesh->CastShadow = false;
	RootComponent = EditorOnlyMesh;

}

// Called when the game starts or when spawned
void ARespawnPoint::BeginPlay()
{
	Super::BeginPlay();

}

void ARespawnPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	URespawnSubsystem* respawnSystem = URespawnSubsystem::GetInst(this);
	if (respawnSystem)
	{
		respawnSystem->RegisterRespawnLocation(District, GetTransform());
	}
}

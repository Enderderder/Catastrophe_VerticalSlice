// Fill out your copyright notice in the Description page of Project Settings.


#include "Tomato.h"

#include "Components/StaticMeshComponent.h"

#include "Characters/GuardCharacter/Guard.h"

// Sets default values
ATomato::ATomato()
{
 	// Set this actor to not call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetCollisionProfileName(TEXT("Throwable"));
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ATomato::OnTomatoOverlap);
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void ATomato::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATomato::OnTomatoOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If the tomato hits the head of a guard
	if (OtherActor->IsA<AGuard>() && OtherComp->ComponentHasTag(TEXT("Head")))
	{
		AGuard* guard = Cast<AGuard>(OtherActor);
		guard->SetGuardState(EGuardState::STUNED);
	}

	// TODO: Spawn decal

	// Destroy the actor
	Destroy();
}

void ATomato::LaunchTomato(FVector _launchDirection, float _launchForce)
{
	//Mesh->AddForce()



	// TODO: Apply force when launch
}

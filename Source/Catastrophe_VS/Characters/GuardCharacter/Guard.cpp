// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;




}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

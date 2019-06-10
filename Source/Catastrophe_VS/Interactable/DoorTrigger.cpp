// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorTrigger.h"

#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

// Sets default values
ADoorTrigger::ADoorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoorTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADoorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorTrigger::ActivateDoor()
{
	Receive_ActivateDoorTrigger();
}

void ADoorTrigger::SetDoorActive(bool _b)
{
	Recieve_SetDoorActive(_b);
}

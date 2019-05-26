// Fill out your copyright notice in the Description page of Project Settings.


#include "TomatoSack.h"

// Sets default values for this component's properties
UTomatoSack::UTomatoSack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTomatoSack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTomatoSack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTomatoSack::AddTomato()
{
	if (TomatoAmount < MaxTomatoAmount)
	{
		TomatoAmount++;
	}
}

void UTomatoSack::AddTomatoes(uint8 _Amount)
{
	if (TomatoAmount + _Amount <= MaxTomatoAmount)
	{
		TomatoAmount += _Amount;
	}
}

void UTomatoSack::FillTomatoSack()
{
	TomatoAmount = MaxTomatoAmount;
}

bool UTomatoSack::IsTomatoSackFull()
{
	return (TomatoAmount == MaxTomatoAmount);
}

void UTomatoSack::RemoveTomato()
{
	if (TomatoAmount > 0)
	{
		TomatoAmount--;
	}
}

void UTomatoSack::RemoveTomatoes(uint8 _Amount)
{
	if (TomatoAmount - _Amount >= 0)
	{
		TomatoAmount -= _Amount;
	}
}

void UTomatoSack::EmptyTomatoSack()
{
	TomatoAmount = 0;
}

bool UTomatoSack::IsTomatoSackEmpty()
{
	return (TomatoAmount == 0);
}

void UTomatoSack::SetTomatoAmount(uint8 _Amount)
{
	TomatoAmount = _Amount;
}

uint8 UTomatoSack::GetTomatoAmount()
{
	return (TomatoAmount);
}

void UTomatoSack::SetSackSize(uint8 _Size)
{
	MaxTomatoAmount = _Size;
}

uint8 UTomatoSack::GetSackSize()
{
	return(MaxTomatoAmount);
}

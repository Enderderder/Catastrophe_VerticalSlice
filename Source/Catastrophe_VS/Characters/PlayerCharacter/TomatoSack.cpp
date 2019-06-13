// Fill out your copyright notice in the Description page of Project Settings.

#include "TomatoSack.h"

// Sets default values for this component's properties
UTomatoSack::UTomatoSack()
{
	// No tick component
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UTomatoSack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTomatoSack::AddTomato()
{
	if (TomatoAmount < MaxTomatoAmount)
	{
		TomatoAmount++;
	}
}

void UTomatoSack::AddTomatoes(uint8 _amount)
{
	TomatoAmount = (uint8)FMath::Min(
		(int32)(TomatoAmount + _amount), (int32)MaxTomatoAmount);
}

void UTomatoSack::FillTomatoSack()
{
	TomatoAmount = MaxTomatoAmount;
}

bool UTomatoSack::IsTomatoSackFull() const
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

void UTomatoSack::RemoveTomatoes(uint8 _amount)
{
	TomatoAmount = (uint8)FMath::Max(
		(int32)(TomatoAmount - _amount), 0);
}

void UTomatoSack::EmptyTomatoSack()
{
	TomatoAmount = 0;
}

bool UTomatoSack::IsTomatoSackEmpty() const
{ 
	return (TomatoAmount <= 0);
}

void UTomatoSack::SetTomatoAmount(uint8 _amount)
{
	TomatoAmount = _amount;
}

uint8 UTomatoSack::GetTomatoAmount() const
{
	return (TomatoAmount);
}

void UTomatoSack::SetSackSize(uint8 _size)
{
	MaxTomatoAmount = _size;
}

uint8 UTomatoSack::GetSackSize() const
{
	return (MaxTomatoAmount);
}

bool UTomatoSack::IsAbleToThrow()
{
	return (TomatoAmount > 0);
}

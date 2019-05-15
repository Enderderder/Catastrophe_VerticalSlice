// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GuardAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API UGuardAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MoveSpeed;

public:
	/** Called on the  */
	virtual void NativeBeginPlay() override;

};

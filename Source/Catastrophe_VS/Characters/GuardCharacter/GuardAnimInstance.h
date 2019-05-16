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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GuardAnim")
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GuardAnim")
	bool bStuned;
	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GuardAnim")
	class AGuard* GuardRef;

protected:
	/** Called on the beginplay of the actor */
	virtual void NativeBeginPlay() override;

public:
	/** Called on each animation update tick */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	/** Getter */
	FORCEINLINE class AGuard* GetGuardCharacterOwner() const {
		return GuardRef;
	}
	/** Getter End */
};

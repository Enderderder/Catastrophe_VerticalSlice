// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bInAir;

	// Temp
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bWalking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bAiming;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	class APlayerCharacter* CustomPlayerCharacter;
	

protected:
	// Executed when begin play is called on the owning component
	virtual void NativeBeginPlay() override;

public:
	// Executed at tick
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	/** Custom Getter */
	FORCEINLINE class APlayerCharacter* GetCustomPlayerCharacterOwner() const {
		return CustomPlayerCharacter;
	}
	/** Getter End */
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "GuardAiController.generated.h"

/**
 * The controller for guard character
 */
UCLASS()
class CATASTROPHE_VS_API AGuardAiController : public AAIController
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Hearing* HearingConfig;

public:
	AGuardAiController();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardAi")
	class AGuard* GuardRef;

protected:
	/** Called when the controller posses a character */
	virtual void OnPossess(APawn* InPawn) override;

	/** Called on delegate function AITargetPerceptionUpdate */
	UFUNCTION()
	void TargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	/** Called on delegate function AIPerceptionUpdate */
	UFUNCTION()
	void PerceptionUpdate(const TArray<AActor*>& UpdatedActors);

public:

	

};

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
	class UAISenseConfig_Sight* SightDefaultConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Hearing* HearingDefaultConfig;

public:
	/** Default constructor */
	AGuardAiController();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardAi")
	class AGuard* ControllingGuard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GuardAi")
	class UBehaviorTree* GuardBehaviourTree;


protected:
	/** Called when the controller posses a character */
	virtual void OnPossess(APawn* InPawn) override;

	/** Called on delegate function AIPerceptionUpdate */
	UFUNCTION()
	void PerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	/** Called when Ai character catches a visual stimulus source */
	virtual void OnSightPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus);

	/** Called when Ai character catches a sound stimulus source */
	virtual void OnHearingPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus);

public:

	/**
	 * Modify the guard sight perception range
	 * @param Range of the sight
	 * @param Range on top of the sight range where perception will lose sight
	 * @note This will overwrite the current value, make sure to store the old value
	 */
	UFUNCTION(BlueprintCallable, Category = "GuardAi")
	bool ModifySightRange(float _newSightRange, float _losingSightRange = 500.0f);

	/** Getter */
	FORCEINLINE UAISenseConfig_Sight* GetSightDefaultConfig() const { return SightDefaultConfig; }
	FORCEINLINE UAISenseConfig_Hearing* GetHearingDefaultConfig() const { return HearingDefaultConfig; }
	/** Getter End */

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "Guard.generated.h"

/**
 * The enum that stores the state of the guard character
 */
UENUM(BlueprintType)
enum class EGuardState : uint8 
{
	STATIONARY = 0,
	SLEEPING,
	WAKEUPSTATEONE,
	WAKEUPSTATETWO,
	PATROLLING,
	INVESTATING,
	CHASING,
	STUNED,
};

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API AGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuard();

	/** Determine if the guard will walk around in his patrol location */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Behaviour")
	bool bPatrolBehaviour = false;

	/** The patrol way points of the guard, need to enable PatrolBehaviour to use them */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard | Behaviour", meta = (MakeEditWidget = "true"))
	TArray<FVector> PatrolLocations;

	/** Previous state of the vision on player */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard | Behaviour")
	bool bPlayerWasInSight = false;

	/** Vision is on the player */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard | Behaviour")
	bool bPlayerInSight = false;

	/** Is guard stuned which will disable movement */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard | Behaviour")
	bool bStuned;

	/** Store the state of the guard */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard | Behaviour")
	EGuardState GuardState;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats")
	float PatrolSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats")
	float ChaseSpeed = 1000.0f;


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** BlueprintNativeEvent that gets the perception location and rotation of the character */
	UFUNCTION(BlueprintNativeEvent, Category = "AIPerception | Character")
	void GetPerceptionLocRot(FVector& Location, FRotator& Rotation) const;
	void GetPerceptionLocRot_Implementation(FVector& Location, FRotator& Rotation) const;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called to get the eye view point of the character */
	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	/** Called when Ai character catches a visual stimulus source */
	virtual void OnSightPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus);

	/** Called when Ai character catches a sound stimulus source */
	virtual void OnHearingPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus);

	/**
	 * Sets the guards maximum walk speed
	 * @Note This will overwrite the character movement component MaxWalkSpeed
	 */
	UFUNCTION(BlueprintCallable, Category = "Guard | Stats")
	void SetGuardMaxSpeed(float _speed);
};

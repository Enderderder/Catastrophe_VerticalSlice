// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "Guard.generated.h"


class UPrimitiveComponent;


/**
 * The enum that stores the state of the guard character
 */
UENUM(BlueprintType)
enum class EGuardState : uint8 
{
	STATIONARY = 0,
	SLEEPING,
	WAKEUP_STAGEONE,
	WAKEUP_STAGETWO,
	PATROLLING,
	INVESTATING,
	CHASING,
	SEARCHING,
	STUNED,
};

/**
 * This character is the main enemy that trying to guard around places and catches the player
 */
UCLASS()
class CATASTROPHE_VS_API AGuard : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardComponents", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* HeadHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardComponents", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BodyHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardComponents", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CatchHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardComponents", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* StupidFakeBsHearingSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardComponents", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* AlertMarkMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardComponents", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* QuestionMarkMesh;

public:
	// Sets default values for this character's properties
	AGuard();

	/** The default state of the guard when it spawns in to the world */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard | Behaviour | General")
	EGuardState DefaultGuardState = EGuardState::STATIONARY;

	/** Determine if the guard will walk around in his patrol location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard | Behaviour | Patrol")
	bool bPatrolBehaviour = false;

	/** The patrol way points of the guard, need to enable PatrolBehaviour to use them */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard | Behaviour | Patrol", meta = (MakeEditWidget = "true"))
	TArray<FVector> PatrolLocations;

	/** Previous state of the vision on player */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard | Behaviour | General")
	bool bPlayerWasInSight = false;

	/** Vision is on the player */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard | Behaviour | General")
	bool bPlayerInSight = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard | Behaviour | Sleep")
	bool bPlayerInSleepDetectRange = false;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Guard | Controller")
	class AGuardAiController* GuardController;

	UPROPERTY(BlueprintReadWrite, Category = "Guard | Animation")
	class UGuardAnimInstance* GuardAnimInstance;

	/**  */
	UPROPERTY(BlueprintReadWrite, Category = "Guard | General")
	class APlayerCharacter* PlayerRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats | Movement")
	float PatrolSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats | Movement")
	float ChaseSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats | Perception")
	float PatrolSightRange = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats | Perception")
	float ChasingSightRange = 2500.0f;
	
	/** This is the range on top of the sight range */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats | Perception")
	float LosingSightRange = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats | Stun")
	float MaxStunTime = 5.0f;

	/** This is the min radius each time when the guard perform a random location search  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats | Searching")
	float SearchRadiusMin = 100.0f;

	/** This is the max radius each time when the guard perform a random location search  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard | Stats | Searching")
	float SearchRadiusMax = 150.0f;


private:

	/** Store the state of the guard */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Guard | Behaviour | General", meta = (AllowPrivateAccess = "true"))
	EGuardState GuardState;

	/** The timer handle for stun mechanic */
	UPROPERTY(BlueprintReadOnly, Category = "Guard | Behaviour | Stun", meta = (AllowPrivateAccess = "true"))
	FTimerHandle StunTimerHnadle;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** BlueprintNativeEvent that gets the perception location and rotation of the character */
	UFUNCTION(BlueprintNativeEvent, Category = "AIPerception | Character")
	void GetPerceptionLocRot(FVector& Location, FRotator& Rotation) const;
	virtual void GetPerceptionLocRot_Implementation(FVector& Location, FRotator& Rotation) const;

	/** Called when the state of the guard changes */
	UFUNCTION(BlueprintNativeEvent, Category = "Guard | Behaviour | General")
	void OnGuardStateChange(EGuardState _oldState, EGuardState _newState);
	virtual void OnGuardStateChange_Implementation(EGuardState _oldState, EGuardState _newState);

	/** Called when guard state switch to stun */
	UFUNCTION()
	virtual void OnStunBegin();
	UFUNCTION(BlueprintImplementableEvent, Category = "Guard | Behaviour | Stun", meta = (DisplayName = "OnStunBegin"))
	void Receive_OnStunBegin();

	/** Called when guard state switch to stun */
	UFUNCTION()
	virtual void OnStunEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Guard | Behaviour | Stun", meta = (DisplayName = "OnStunEnd"))
	void Receive_OnStunEnd();

	/** Called when the catch hit box overlap */
	UFUNCTION()
	virtual void OnCatchHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
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
	 * Sets the state of the guard then modify the character value base on the state
	 * @param The new state of the guard
	 */
	UFUNCTION(BlueprintCallable, Category = "Guard | Behaviour | General")
	void SetGuardState(EGuardState _newState);

	/**
	 * Sets the guards maximum walk speed
	 * @note This will overwrite the character movement component MaxWalkSpeed
	 */
	UFUNCTION(BlueprintCallable, Category = "Guard | Stats")
	void SetGuardMaxSpeed(float _speed);

	/** BLueprint function, Show the alert indicator or not */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Guard | Animation")
	void ToggleAlertIndicator(bool _b);

	/** BLueprint function, Show the question mark indicator or not */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Guard | Animation")
	void ToggleQuestionIndicator(bool _b);

	/** Stops all the montages that this character is playing */
	UFUNCTION(BlueprintCallable, Category = "Guard | Animation")
	void StopAllMontages();

	/** Look around behaviour */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Guard | Animation")
	void LookAround(float& out_montageTime);
	virtual void LookAround_Implementation(float& out_montageTime);


	/** Setter */
	void SetGuardControllerRef(class AGuardAiController* _controller) {
		GuardController = _controller;
	}
	/** Setter End */

	/** Getter */
	FORCEINLINE EGuardState GetGuardState() const { return GuardState; }
	FORCEINLINE class AGuardAiController* GetGuardController() const { return GuardController; }
	/** Getter End */
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


/**
 * HHU (Hand Hold Utility) types
 * This is the player skill set on which will cast through hand
 */
UENUM(BlueprintType)
enum class EHHUType : uint8
{
	TOMATO,
	LASER,
};

/**
 * This struct stores the default value of the player
 */
USTRUCT(BlueprintType)
struct FPlayerDefaultValue
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraArmLength;

	FPlayerDefaultValue() :
		WalkSpeed(0.0f),
		CameraFOV(100.0f),
		CameraArmLength(650.0f)
	{}
};


/**
 * This class is the core of the game. Contains all the information and functionality
 * for the player character
 */
UCLASS()
class CATASTROPHE_VS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Where the camera is going to be focused on normally */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CamFocusPoint;

	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* ZoomInTimeline;

	/** Where the camera is going to be focused on during aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* AimDownSightFocusPoint;

	// Deprecated TODO: Remove reference of this component
	/** Spawn loaction for the throwable */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TomatoSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TomatoInHandMesh;

	// Deprecated TODO: Remove reference of this component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GoalFish", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FishToCarry;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

protected:

	/** Player animation instance */
	UPROPERTY(BlueprintReadOnly, Category = "Player | Animation")
	class UPlayerAnimInstance* PlayerAnimInstance;

	/** Origin values of the player */
	UPROPERTY(BlueprintReadOnly, Category = "Player | General")
	FPlayerDefaultValue PlayerDefaultValues;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.0f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.0f;


	/** The current interacble target */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interaction")
	class AActor* InteractTarget;


	/** Currently activated HHU(Hand Hold Utility) */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HHU | General")
	EHHUType ActiveHHUType = EHHUType::TOMATO;

	/** Is HHU(Hand Hold Utility) primary action active */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HHU | General")
	bool bHHUPrimaryActive = false;

	/** Is HHU(Hand Hold Utility) secondary action active */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HHU | General")
	bool bHHUSecondaryActive = false;

	/** The HHU zoom curve */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | General")
	class UCurveFloat* ZoomInCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | General")
	float CameraZoomMultiplier;

	/** Class object that define what object will be throw out as tomato */
	UPROPERTY(EditDefaultsOnly, Category = "HHU | Tomato")
	TSubclassOf<class ATomato> TomatoClass;

	/** The total amount of tomato player can hold */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HHU | Tomato")
	int32 TomatoTotalCount = 1;

	/** The force apply when player throw the tomato */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HHU | Tomato")
	float TomatoLaunchForce = 100.0f;


	/** Sprint & Stamina */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float TotalStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float StaminaDrainPerSec = 10.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Movement")
	float CurrentStamina;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Movement")
	bool bSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SpringSpeedMultiplier = 1.3f;

	/** Determine if the player can control the movement of the character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	bool bAllowMovementInput = true;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HHU | Tomato")
	int32 TomatoCurrentCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HHU | General")
	bool bCanUseHHU = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Controller Action

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Called for character sprinting begin */
	void SprintBegin();

	/** Called for character sprinting end */
	void SprintEnd();

	/** Called for character crouching begin */
	void CrouchBegin();

	/** Called for character crouching end */
	void CrouchEnd();

	/** Called when the interacation button pressed */
	void InteractBegin();

	/** Called when the interaction button released */
	void InteractEnd();

	/** HHD(Hand Hold Utility) primary action begin */
	void HHUPrimaryActionBegin();

	/** HHD(Hand Hold Utility) primary action end */
	void HHUPrimaryActionEnd();

	/** HHD(Hand Hold Utility) secondary action begin */
	void HHUSecondaryActionBegin();

	/** HHD(Hand Hold Utility) secondary action end */
	void HHUSecondaryActionEnd();

#pragma endregion Controller Action

	/** Check if player has tomato in his hand */
	void CheckTomatoInHand();

	/** Called for shooting shuriken */
	UFUNCTION(BlueprintCallable, Category = "Skill_Tomato")
	void ShootTomato();

	/** Called when ZoomInTimeline ticks */
	UFUNCTION()
	void TimelineSetCameraZoomValue(float _alpha);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for set the kunai count to maximum */
	UFUNCTION(BlueprintCallable, Category = "HHU | Tomato")
	void RestoreAllTomatos();

	/** Called for restore certain amount of kunai (but not over the max) */
	UFUNCTION(BlueprintCallable, Category = "HHU | Tomato")
	void RestoreTomato(int32 _count);

	/** Return the current count of kunai player is holding */
	UFUNCTION(BlueprintCallable, Category = "HHU | Tomato")
	int GetTomatoCount() { return TomatoCurrentCount; }

	UFUNCTION(BlueprintCallable, Category = "Fish")
	void GrabbingFish();

	/** Set the target to interact for the player */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractionTarget(class AActor* _interactTarget);

	/** Try to remove the interaction target if it exists */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RemoveInteractionTarget(class AActor* _interactTarget);

	/**
	 * Set the value of current stamina
	 * @note This will not overflow the stamina
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetStamina(float _value);
	
	/**
	 * Called to stop all the movement that the player currently has
	 * @param Option to block player movement input
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void BlockMovementAction(bool _bBlockMovementInput);

	/** Allow player take movement control again */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UnblockMovementInput();


	/** Getter */
	FORCEINLINE class UAIPerceptionStimuliSourceComponent* GetStimulusSourceComponent() const { 
		return PerceptionStimuliSourceComponent; }
	FORCEINLINE float GetTotalStamina() const { return TotalStamina; }
	/** Getter End */

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	LEFT,
	RIGHT,
	FORWARD,
	BACKWARD
};

/**
 * HHU (Hand Hold Utility) types
 * This is the player skill set on which will cast through hand
 */
UENUM(BlueprintType)
enum class EHHUType : uint8
{
	THROWING,
	LASER,
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

	/** Where the camera is going to be focused on during aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* AimDownSightFocusPoint;

	// Deprecated TODO: Remove reference of this component
	/** Spawn loaction for the throwable */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TomatoSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TomatoInHandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GoalFish", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FishToCarry;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

protected:

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
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HHU | General")
	EHHUType ActiveHHUType;

	/** Is HHU(Hand Hold Utility) primary action active */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HHU | General")
	bool bHHUPrimaryActive = false;

	/** Is HHU(Hand Hold Utility) secondary action active */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HHU | General")
	bool bHHUSecondaryActive = false;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** Class object that define what object will be throw out as tomato */
	UPROPERTY(EditDefaultsOnly, Category = "HHU | Throwable")
	TSubclassOf<class AActor> TomatoClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | Tomato")
	int32 TomatoTotalCount = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "HHU | Tomato")
	int32 TomatoCurrentCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | General")
	float CameraZoomRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HHU | General")
	bool bCanUseHHU = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
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

	/** Called for aim down sight for shooting tomato */
	void AimDownSight();
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill_Tomato", meta = (DisplayName = "OnAimDownSight"))
	void Receive_AimDownSight();

	/** Called for exite aim down sight */
	void ExitAimDownSight();
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill_Tomato", meta = (DisplayName = "OnExitAimDownSight"))
	void Receive_ExitAimDownSight();

	/** Called for shooting shuriken */
	UFUNCTION(BlueprintCallable, Category = "Skill_Tomato")
	void ShootTomato();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for set the kunai count to maximum */
	UFUNCTION(BlueprintCallable, Category = "Skill_Tomato")
	void RestoreAllTomatos();

	/** Called for restore certain amount of kunai (but not over the max) */
	UFUNCTION(BlueprintCallable, Category = "Skill_Tomato")
	void RestoreTomato(int _count);

	/** Return the current count of kunai player is holding */
	UFUNCTION(BlueprintCallable, Category = "Skill_Tomato")
	int GetKunaiCount() { return TomatoCurrentCount; }

	UFUNCTION(BlueprintCallable, Category = "Fish")
	void GrabbingFish();

	/** Set the target to interact for the player */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractionTarget(class AActor* _interactTarget);

	/** Try to remove the interaction target if it exists */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RemoveInteractionTarget(class AActor* _interactTarget);



	/** Getter */
	FORCEINLINE class UAIPerceptionStimuliSourceComponent* GetStimulusSourceComponent() const { 
		return PerceptionStimuliSourceComponent; }
	/** Getter End */

};

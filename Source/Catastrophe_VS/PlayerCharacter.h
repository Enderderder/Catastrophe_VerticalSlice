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

UCLASS()
class CATASTROPHE_VS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TomatoInHandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GoalFish", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FishToCarry;

#pragma region CameraComponents

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

#pragma endregion CameraComponents

	/** Spawn loaction for the shuriken */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TomatoSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* FollowCameraFocusPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* AimDownSightFocusPoint;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

#pragma region CameraStats
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<class ACameraActor> BirdViewCameraClass;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	bool bIsBirdEyeCamera;

#pragma endregion CameraStats

#pragma region Tomato

	UPROPERTY(EditAnywhere, Category = "Skill_Tomato")
	TSubclassOf<class APawn> TomatoObject;

	UPROPERTY(EditDefaultsOnly, Category = "Skill_Tomato")
	int TomatoTotalCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato")
	int TomatoCurrentCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill_Tomato")
	bool AimDownSightState;

	UPROPERTY(EditDefaultsOnly, Category = "Skill_Tomato")
	float CameraZoomRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill_Tomato")
	bool bAbleToShootTomato;

#pragma endregion Tomato

#pragma region Interaction

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class AInteractActor* InteractTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bAbleToInterate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bOpenToInteract;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bInteracting;

#pragma endregion Interaction

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Toggle the camera */
	void CameraToggle();

	UFUNCTION(BlueprintImplementableEvent, Category = Camera, meta = (DisplayName = "OnCameraToggle"))
	void ReceiveCameraToggle();

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

	/** Called for character crouching input */
	void StartCrouch();

	/** Called for character crouching input */
	void EndCrouch();

	/** Check if player has tomato in his hand */
	void CheckTomatoInHand();

	/** Called for aim down sight for shooting tomato */
	void AimDownSight();

	/** Called for exite aim down sight */
	void ExitAimDownSight();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skill_Tomato", meta = (DisplayName = "OnAimDownSight"))
	void ReceiveAimDownSight();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skill_Tomato", meta = (DisplayName = "OnExitAimDownSight"))
	void ReceiveExitAimDownSight();

	/** Called for shooting shuriken */
	UFUNCTION(BlueprintCallable, Category = "Skill_Tomato")
	void ShootTomato();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractAction();

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
	void SetInteractionTarget(class AInteractActor* _interactTarget);

	/** Try to remove the interaction target if it exists */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RemoveInteractionTarget(class AInteractActor* _interactTarget);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OpenToInteraction();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CloseInteraction();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsOpenToInteract() { return bOpenToInteract; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsPlayerInteracting() { return bInteracting; }



};

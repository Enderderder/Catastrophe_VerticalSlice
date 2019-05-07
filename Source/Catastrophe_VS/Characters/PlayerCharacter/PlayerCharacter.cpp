// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "Interactable/InteractActor.h"
#include "Interactable/BaseClasses/InteractableObject.h"

//#include "Engine.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Set the tomato that will show inside players hand
	TomatoInHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TomatoInHandMesh"));
	TomatoInHandMesh->SetupAttachment(GetMesh(), TEXT("TomatoSocket"));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	FollowCameraFocusPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FollowCameraFocusPoint"));
	FollowCameraFocusPoint->SetupAttachment(GetMesh());

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

// 	// Create a stationary camera that sits high above the player
// 	BirdEyeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BirdCamera"));

	TomatoSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TomatoSpawnPoint"));
	TomatoSpawnPoint->SetupAttachment(GetMesh());

	AimDownSightFocusPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AimDownSightFocusPoint"));
	AimDownSightFocusPoint->SetupAttachment(GetMesh());

	FishToCarry = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FishToCarry"));
	FishToCarry->SetupAttachment(GetMesh(), TEXT("FishBackSocket"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Attach the camera to the focus point
	CameraBoom->AttachToComponent(FollowCameraFocusPoint, FAttachmentTransformRules::KeepRelativeTransform);

	// Make sure the bird eye camera is not being use at the beignning
	bIsBirdEyeCamera = false;

	// Default Interaction state
	bAbleToShootTomato = true;

	CheckTomatoInHand();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	/// To use the tick function, go to the constructor 
	/// and uncomment the " PrimaryActorTick.bCanEverTick = true; "
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::EndCrouch);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	// Player other action (Interactions and Functionalities)
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::InteractAction);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::InteractActionEnd);
	PlayerInputComponent->BindAction("BirdViewToggle", IE_Pressed, this, &APlayerCharacter::CameraToggle);
	PlayerInputComponent->BindAction("AimDownSight", IE_Pressed, this, &APlayerCharacter::AimDownSight);
	PlayerInputComponent->BindAction("AimDownSight", IE_Released, this, &APlayerCharacter::ExitAimDownSight);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerCharacter::ShootTomato);
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::StartCrouch()
{
	Crouch();
}

void APlayerCharacter::EndCrouch()
{
	UnCrouch();
}

void APlayerCharacter::CheckTomatoInHand()
{
	if (TomatoCurrentCount > 0)
	{
		TomatoInHandMesh->SetVisibility(true);
	}
	else
	{
		TomatoInHandMesh->SetVisibility(false);
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::CameraToggle()
{
	// Blueprint implement event
	ReceiveCameraToggle();

	// Do the flip flop
	bIsBirdEyeCamera = !bIsBirdEyeCamera;
	if (bIsBirdEyeCamera)
	{
		// Disable the movement
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		bAbleToShootTomato = false;

		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		if (playerController)
		{
			AActor* birdViewCameraAcrtor;
			TArray<AActor*> resultActorArray;
			UGameplayStatics::GetAllActorsOfClass(this, BirdViewCameraClass, resultActorArray);
			if (resultActorArray.Num() != 0)
			{
				birdViewCameraAcrtor = resultActorArray[0];
				playerController->SetViewTargetWithBlend(birdViewCameraAcrtor, 1.0f);
			}
		}
	}
	else
	{
		// Re-enable the movement
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bAbleToShootTomato = true;

		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		if (playerController)
		{
			playerController->SetViewTargetWithBlend(this, 1.0f);
		}
	}
}

void APlayerCharacter::AimDownSight()
{
	// Abort when player cant shoot tomato
	if (!bAbleToShootTomato)
		return;

	// Change the ADS state to true
	AimDownSightState = true;

	// Let the character follow camera rotation
	bUseControllerRotationYaw = true;

	CameraBoom->AttachToComponent(AimDownSightFocusPoint, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->TargetArmLength *= CameraZoomRatio;

	// Call the blueprint implemented event
	Receive_AimDownSight();
}

void APlayerCharacter::ExitAimDownSight()
{
	// Abort when ads state is not even set
	if (!AimDownSightState)
		return;

	// Change the ADS state to false
	AimDownSightState = false;

	// Let the character not follow camera rotation
	bUseControllerRotationYaw = false;

	CameraBoom->AttachToComponent(FollowCameraFocusPoint, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->TargetArmLength /= CameraZoomRatio;

	// Call the blueprint implemented event
	Receive_ExitAimDownSight();
}

void APlayerCharacter::ShootTomato()
{
	// Validate the obejct pointer
	// Check if the player is ADSing
	// Check if there is enough tomato to shoot
	if (TomatoObject 
		&& AimDownSightState == true
		&& bAbleToShootTomato == true
		&& TomatoCurrentCount > 0)
	{
		// Set the parameter for spawning the shuriken
		FVector tomatoSpawnLocation;
		FRotator tomatoSpawnRotation;
		FActorSpawnParameters tomatoSpawnInfo;
		tomatoSpawnInfo.Owner = this;
		tomatoSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		tomatoSpawnLocation = TomatoSpawnPoint->GetComponentLocation();
		tomatoSpawnRotation = FollowCamera->GetComponentRotation();

		// Spawn the tomato
		APawn* SpawnedTomato;
		SpawnedTomato = GetWorld()->SpawnActor<APawn>(TomatoObject, tomatoSpawnLocation, tomatoSpawnRotation, tomatoSpawnInfo);

		// Lower the ammo
		TomatoCurrentCount--;

		// Check if theres tomato left in the hand
		CheckTomatoInHand();
	}
}

void APlayerCharacter::InteractAction()
{
	if (InteractTarget && !InteractTarget->IsPendingKill())
	{
		if (InteractTarget->GetClass()->ImplementsInterface(UInteractableObject::StaticClass()))
		{
			IInteractableObject::Execute_OnInteract(InteractTarget, this);
			RemoveInteractionTarget(InteractTarget);
		}
	}
}

void APlayerCharacter::InteractActionEnd()
{

}

void APlayerCharacter::RestoreAllTomatos()
{
	TomatoCurrentCount = TomatoTotalCount;
	CheckTomatoInHand();
}

void APlayerCharacter::RestoreTomato(int _count)
{
	TomatoCurrentCount = FMath::Min(TomatoTotalCount, TomatoCurrentCount + _count);
	CheckTomatoInHand();
}

void APlayerCharacter::GrabbingFish()
{
	// Set the visibility of the fish to visible
	FishToCarry->SetVisibility(true);
}

void APlayerCharacter::SetInteractionTarget(class AActor* _interactTarget)
{
	if (auto interactableInterface = Cast<IInteractableObject>(_interactTarget))
	{
		InteractTarget = _interactTarget;
	}
}

void APlayerCharacter::RemoveInteractionTarget(class AActor* _interactTarget)
{
	if (_interactTarget == InteractTarget)
	{
		InteractTarget = nullptr;
	}
}


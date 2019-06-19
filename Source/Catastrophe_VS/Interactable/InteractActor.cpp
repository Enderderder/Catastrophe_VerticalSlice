// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractActor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"

// Sets default values
AInteractActor::AInteractActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	// The core component of the interaction system
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetupAttachment(RootComponent);

	// Set this interaction actor to able to interact by player at the beginnig
	bUseable = true;
}

void AInteractActor::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnTriggerEnterCheck(OtherActor);
}

void AInteractActor::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnTriggerrExitCheck(OtherActor);
}

// Called when the game starts or when spawned
void AInteractActor::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractActor::OnTriggerEnter);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractActor::OnTriggerExit);

	// Try to find the player reference
	PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AInteractActor::OnTriggerEnterCheck(class AActor* _otherActor)
{
	if (PlayerReference && Cast<APlayerCharacter>(_otherActor) == PlayerReference && bUseable)
	{
		//PlayerReference->SetInteractionTarget(this);
	}
}

void AInteractActor::OnTriggerrExitCheck(class AActor* _otherActor)
{
	if (PlayerReference && Cast<APlayerCharacter>(_otherActor) == PlayerReference)
	{
		//PlayerReference->RemoveInteractionTarget(this);
	}
}

// Called every frame
void AInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the player ref is not valid, try again
	if (!PlayerReference)
	{
		PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
}

void AInteractActor::OnInteractionStart()
{
	ReciveInteractionStart();
}

void AInteractActor::OnInteractionEnd()
{
	ReciveInteractionEnd();
}

void AInteractActor::BeginInteraction()
{
	OnInteractionStart();
}

void AInteractActor::EndInteraction()
{
	OnInteractionEnd();
}

void AInteractActor::DisableInteractActor()
{
	SetActorTickEnabled(false);
	TriggerBox->SetGenerateOverlapEvents(false);
}


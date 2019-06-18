// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interactable/BaseClasses/InteractableComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(root);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->RegisterTriggerVolume(TriggerBox);
	InteractableComponent->OnInteract.AddDynamic(this, &AShop::Interact);
}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();

	bIsActivated = false;
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShop::WidgetInit(TSubclassOf<class UUserWidget> _Widget)
{
	if (_Widget)
	{
		ShopWidget = CreateWidget<UUserWidget>(GetWorld(), _Widget);
	}
}

void AShop::Interact(class APlayerCharacter* _playerCharacter)
{
	if (!bIsActivated)
	{
		EnableShop();
	}
	else
	{
		DisableShop();
	}

	Receive_Interact();
}

void AShop::EnableShop()
{
	if (ShopWidget)
	{
		Receive_EnableShop();
		ShopWidget->AddToViewport();
		bIsActivated = true;
	}
}

void AShop::DisableShop()
{
	if (ShopWidget)
	{
		Receive_DisableShop();
		ShopWidget->RemoveFromParent();
		bIsActivated = false;
	}
}

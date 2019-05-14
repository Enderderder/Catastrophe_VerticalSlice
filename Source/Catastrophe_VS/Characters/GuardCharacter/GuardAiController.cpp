// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardAiController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

#include "Guard.h"

AGuardAiController::AGuardAiController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGuardAiController::TargetPerceptionUpdate);
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AGuardAiController::PerceptionUpdate);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*SightConfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*HearingConfig);
}

void AGuardAiController::OnPossess(APawn* InPawn)
{
	// Sets the reference of the guard
	GuardRef = Cast<AGuard>(InPawn);


	Super::OnPossess(InPawn);
}

void AGuardAiController::TargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	
}

void AGuardAiController::PerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo actorPerceptionInfo;
		if (PerceptionComponent->GetActorsPerception(actor, actorPerceptionInfo))
		{
			//for (FAIStimulus stimulus : )
		}
	}
}

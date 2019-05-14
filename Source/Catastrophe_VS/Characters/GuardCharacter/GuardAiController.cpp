// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardAiController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

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

void AGuardAiController::PerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo actorPerceptionInfo;
		if (PerceptionComponent->GetActorsPerception(actor, actorPerceptionInfo))
		{
			for (int32 index = 0; index < actorPerceptionInfo.LastSensedStimuli.Num(); ++index)
			{
				switch (index)
				{
				case 0: // Sight perception updated
					OnSightPerceptionUpdate(actor, actorPerceptionInfo.LastSensedStimuli[index]);
					break;

				case 1: // Hearing perception updated
					OnHearingPerceptionUpdate(actor, actorPerceptionInfo.LastSensedStimuli[index]);
					break;

				default:
					UE_LOG(LogTemp, Warning, TEXT("There is no third sense lol wtf"));
					break;
				}
			}
		}
	}
}

void AGuardAiController::OnSightPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus)
{
	// If updated actor is player
	if (_actor->ActorHasTag(TEXT("Player")))
	{
		if (_stimulus.WasSuccessfullySensed())
		{
			GuardRef->bPlayerWasInSight = true;
			GuardRef->bPlayerInSight = true;

			Blackboard->SetValueAsVector(
				TEXT("PlayerLocation"), _stimulus.StimulusLocation);
			

		}
		else
		{
			GuardRef->bPlayerInSight = false;

			// TODO: Test if player was seen and tell him to chase or not
		}
	}

	// Calls the guard character version of the function
	GuardRef->OnSightPerceptionUpdate(_actor, _stimulus);

}

void AGuardAiController::OnHearingPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus)
{
	// Calls the guard character version of the function
	GuardRef->OnHearingPerceptionUpdate(_actor, _stimulus);

}

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
	//PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGuardAiController::TargetPerceptionUpdate);
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AGuardAiController::PerceptionUpdate);

	SightDefaultConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightDefaultConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightDefaultConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightDefaultConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*SightDefaultConfig);

	HearingDefaultConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	HearingDefaultConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingDefaultConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingDefaultConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*HearingDefaultConfig);
}

void AGuardAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Sets the reference of the guard
	GuardRef = Cast<AGuard>(InPawn);
	if (GuardRef && !GuardRef->IsPendingKill())
	{
		GuardRef->SetGuardControllerRef(this);

		// Runs the behaviour tree of guard
		if (GuardBehaviourTree)
			RunBehaviorTree(GuardBehaviourTree);

		// Sets the origin location of the patrol location if guard has one
		if (Blackboard && GuardRef->PatrolLocations.Num() > 0)
		{
			Blackboard->SetValueAsVector(
				TEXT("PatrolOriginLocation"),
				GuardRef->PatrolLocations[0] + GuardRef->GetActorLocation());

			// Set the patrol behaviour as initial state
			if (GuardRef->bPatrolBehaviour)
			{
				GuardRef->SetGuardState(EGuardState::PATROLLING);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Guard controller is not controlling a guard"));
	}
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
			GuardRef->SetGuardState(EGuardState::CHASING);
		}
		else
		{
			GuardRef->bPlayerInSight = false;

			if (GuardRef->bPlayerWasInSight)
			{
				Blackboard->SetValueAsVector(
					TEXT("PlayerlastSeenLocation"), _stimulus.StimulusLocation);

				// TODO: Have seen player, need to chase
			}
			else
			{
				// Do whatever, hasn't seen player yet
			}
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

bool AGuardAiController::ModifySightRange(float _newRange)
{
	FAISenseID sightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	if (!sightSenseID.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get sight sense ID"));
		return false;
	}

	if (!PerceptionComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("This controller dont have perception component"));
		return false;
	}
	UAISenseConfig_Sight* sightConfig = 
		Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSenseConfig(sightSenseID));
	if (!sightConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sight config found"));
		return false;
	}
	
	sightConfig->SightRadius = _newRange;
	return true;
}

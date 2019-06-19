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
	ControllingGuard = Cast<AGuard>(InPawn);
	if (ControllingGuard && !ControllingGuard->IsPendingKill())
	{
		ControllingGuard->SetGuardControllerRef(this);

		// Runs the behaviour tree of guard
		if (GuardBehaviourTree)
			RunBehaviorTree(GuardBehaviourTree);

		if (Blackboard)
		{
			// Sets the default state of the guard
			ControllingGuard->SetGuardState(ControllingGuard->DefaultGuardState);

			// If sets to patrol but guard dont have patrol behaviour 
			// or dont have patrol points, reset it to stationary
			if (ControllingGuard->GetGuardState() == EGuardState::PATROLLING)
			{
				if (!ControllingGuard->bPatrolBehaviour
					|| ControllingGuard->PatrolLocations.Num() <= 0)
				{
					ControllingGuard->SetGuardState(EGuardState::STATIONARY);
				}
				else
				{
					// Sets the origin location of the patrol location
					Blackboard->SetValueAsVector(
						TEXT("PatrolOriginLocation"),
						ControllingGuard->PatrolLocations[0] + ControllingGuard->GetActorLocation());
				}
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
				if (actorPerceptionInfo.LastSensedStimuli[index].IsValid())
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
}

void AGuardAiController::OnSightPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus)
{
	// If updated actor is player
	if (_actor->ActorHasTag(TEXT("Player")))
	{
		if (_stimulus.WasSuccessfullySensed())
		{
			ControllingGuard->bPlayerInSight = true;
			if (!ControllingGuard->bPlayerWasInSight)
			{
				ControllingGuard->bPlayerWasInSight = true;
				ControllingGuard->SetGuardState(EGuardState::CHASING);
				UE_LOG(LogTemp, Warning, TEXT("I can see uuuuu biotch"));
			}	
		}
		else
		{
			ControllingGuard->bPlayerInSight = false;
			if (ControllingGuard->bPlayerWasInSight)
			{
				// Record the last seen location of the player
				Blackboard->SetValueAsVector(
					TEXT("PlayerlastSeenLocation"), _stimulus.StimulusLocation);

				ControllingGuard->SetGuardState(EGuardState::SEARCHING);
				ControllingGuard->bPlayerWasInSight = false;
			}
			else
			{
				// Do whatever, hasn't seen player yet
			}
		}
	}

	// Calls the guard character version of the function
	ControllingGuard->OnSightPerceptionUpdate(_actor, _stimulus);

}

void AGuardAiController::OnHearingPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus)
{
	// TODO: If not chasing player which is the higher priority task,
	// investigate whatever the sound is

	// If heard player making noise
	if (_actor->ActorHasTag(TEXT("Player")))
	{
		if (_stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("I can hear uuuuu"));
		}
	}


	// Calls the guard character version of the function
	ControllingGuard->OnHearingPerceptionUpdate(_actor, _stimulus);
}

bool AGuardAiController::ModifySightRange(float _newSightRange, float _losingSightRange)
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
	
	// Modify values and tell the config to be updated to the system
	sightConfig->SightRadius = _newSightRange;
	sightConfig->LoseSightRadius = _newSightRange + _losingSightRange;
	PerceptionComponent->RequestStimuliListenerUpdate();

	return true;
}

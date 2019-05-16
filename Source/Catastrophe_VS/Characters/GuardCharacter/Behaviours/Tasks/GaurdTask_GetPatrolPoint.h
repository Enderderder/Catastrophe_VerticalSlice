// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GaurdTask_GetPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API UGaurdTask_GetPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

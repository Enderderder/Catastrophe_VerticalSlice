// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestBase.generated.h"


UENUM(BlueprintType)
enum EQuestState
{
	Locked = 0,
	Avaliable,
	Active,
	Completed,

	StateCount // This is the counter, should always be at the end
};

USTRUCT(BlueprintType)
struct FQuestInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RewardExp;

	FQuestInfo() :
		QuestName("DefaultName"),
		QuestDescription("DefaultDescription"),
		RewardExp(0)
	{}
};

UCLASS()
class CATASTROPHE_VS_API AQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FQuestInfo QuestInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest")
	TEnumAsByte<EQuestState> QuestState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

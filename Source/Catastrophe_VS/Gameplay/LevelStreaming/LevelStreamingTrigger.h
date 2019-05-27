// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelStreamingTrigger.generated.h"


/**
 * This actor is 
 */
UCLASS()
class CATASTROPHE_VS_API ALevelStreamingTrigger : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* EditorOnlyMesh;
	
public:	
	// Sets default values for this actor's properties
	ALevelStreamingTrigger();

protected:

	/** If true, current level will be unloaded */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LevelStreaming")
	bool bShouldUnloadThisLevelAfter = true;

	/** If true, after level loaded, will teleport player to the destination */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LevelStreaming")
	bool bTeleportAfterLoaded = true;

	/** If true, after level loaded, will teleport player to the destination */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LevelStreaming")
	bool bShouldBlockOnLoad = false;

	/**
	 * The name of the level which this actor will be in
	 * @note Make sure to put the actor at persistent level, other wise this will not function properly
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LevelStreaming")
	FName CurrentLevel = "DefaultName";

	/** The name of the target level to load */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LevelStreaming")
	FName DestinationLevel = "DefaultName";

	/**
	 * The relative location to the actor that the player will teleport to when the
	 * level loading is finished
	 * @note This is a relative location! Make sure to convert to world location before use
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LevelStreaming", meta = (MakeEditWidget = "true"))
	FTransform DestinationTransformRelative;

	/** The loading screen that will appear when its loading */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "LevelStreaming")
	TSubclassOf<class UUserWidget> LoadingScreenClass;

	/** Store the widget object after created */
	UPROPERTY(BlueprintReadWrite)
	class UUserWidget* LoadingScreenWidget;

	/** Store the player actor to teleport */
	UPROPERTY(BlueprintReadWrite)
	AActor* PlayerToTeleport;

public:

	UPROPERTY(BlueprintReadWrite, Category = "LevelStreaming")
	bool bActive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when collide with player */
	UFUNCTION()
	void OnPlayerEnterTrigger(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when the current level done loaded */
	UFUNCTION()
	virtual void OnLevelLoaded();

	/** Called when the current level done unloaded */
	UFUNCTION()
	virtual void OnLevelLoadingFullyFinished();
};

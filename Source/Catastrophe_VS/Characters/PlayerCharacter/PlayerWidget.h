// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_VS_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/**
	 * Called to toggle crosshair visibility
	 * @param On/Off
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerWidget")
	void ToggleCrosshair(bool _b);
	virtual void ToggleCrosshair_Implementation(bool _b);

	/**
	 * Called to toggle stamina UI visibility
	 * @param On/Off
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerWidget")
	void ToggleStamina(bool _b);
	virtual void ToggleStamina_Implementation(bool _b);
	
};

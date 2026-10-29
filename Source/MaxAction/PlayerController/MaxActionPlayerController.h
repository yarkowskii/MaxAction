// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MaxActionPlayerController.generated.h"

/**
 *
 */
UCLASS()
class MAXACTION_API AMaxActionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float Health, float MaxHealth);

protected:
	virtual void BeginPlay() override;

private:
	class AMaxActionHUD *HUD;
};

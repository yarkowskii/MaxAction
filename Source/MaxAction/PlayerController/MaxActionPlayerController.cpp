// Fill out your copyright notice in the Description page of Project Settings.

#include "MaxActionPlayerController.h"
#include "MaxAction/HUD/MaxActionHUD.h"
#include "MaxAction/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void AMaxActionPlayerController::BeginPlay()
{
    HUD = Cast<AMaxActionHUD>(GetHUD());
}

void AMaxActionPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
    HUD = HUD == nullptr ? Cast<AMaxActionHUD>(GetHUD()) : HUD;
    bool bHUDValid = HUD &&
                     HUD->CharacterOverlay &&
                     HUD->CharacterOverlay->HealthBar &&
                     HUD->CharacterOverlay->HealthText;
    if (bHUDValid)
    {
        HUD->CharacterOverlay->SetHealth(Health, MaxHealth);
    }
}

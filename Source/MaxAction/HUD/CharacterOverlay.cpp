// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharacterOverlay::SetHealth(float Health, float MaxHealth)
{
    HealthBar->SetPercent(Health / MaxHealth);
    HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth))));
}

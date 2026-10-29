// Fill out your copyright notice in the Description page of Project Settings.

#include "MaxActionHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"

void AMaxActionHUD::DrawHUD()
{
    Super::DrawHUD();
}

void AMaxActionHUD::BeginPlay()
{
    Super::BeginPlay();

    AddCharacterOverlay();
}

void AMaxActionHUD::AddCharacterOverlay()
{
    APlayerController *PlayerController = GetOwningPlayerController();
    if (PlayerController && CharacterOverlayClass)
    {
        CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
        CharacterOverlay->AddToViewport();
    }
}
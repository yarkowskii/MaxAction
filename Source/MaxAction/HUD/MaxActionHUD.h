// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MaxActionHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

	class UTexture2D *CrosshairsCenter;
	UTexture2D *CrosshairsLeft;
	UTexture2D *CrosshairsRight;
	UTexture2D *CrosshairsTop;
	UTexture2D *CrosshairsBottom;
};

UCLASS()
class MAXACTION_API AMaxActionHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	FHUDPackage HUDPackage;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage &Package) { HUDPackage = Package; }
};

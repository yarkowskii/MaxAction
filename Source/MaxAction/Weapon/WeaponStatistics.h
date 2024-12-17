// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponStatistics.generated.h"

USTRUCT(BlueprintType)
struct MAXACTION_API FWeaponStatistics
{
	GENERATED_BODY()

public:
	// Base weapon stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseDamage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float FireRate = 600.0f; // Rounds per minute

	// Accuracy and Recoil
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
	float BaseSpread = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
	float SpreadMultiplier = 1.0f;

	// ADS (Aim Down Sight) properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	float ADSSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	float ADSZoomMultiplier = 1.5f;

	// Ammunition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MagazineCapacity = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float ReloadTime = 2.0f;

	// Handling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Handling")
	float EquipTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Handling")
	float MovementSpeedMultiplier = 1.0f;

	// Constructor
	FWeaponStatistics()
	{
		// Default constructor
	}
};

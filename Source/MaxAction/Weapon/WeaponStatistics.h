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
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float FireRate; // Rounds per minute

	// Accuracy and Recoil
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
	float BaseSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accuracy")
	float SpreadMultiplier;

	// ADS (Aim Down Sight) properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	float ADSSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	float ADSZoomMultiplier = 1.0f;

	// Ammunition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float ReloadTime;

	// Handling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Handling")
	float EquipTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Handling")
	float MovementSpeedMultiplier = 1.0f;

	// Constructor
	FWeaponStatistics()
	{
		// Default constructor
	}
};

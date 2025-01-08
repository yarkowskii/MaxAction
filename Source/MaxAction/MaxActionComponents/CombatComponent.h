// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MaxAction/Weapon/WeaponStatistics.h"
#include "CombatComponent.generated.h"

class ASoldierCharacter;
class AWeapon;
class AMaxActionPlayerController;
class AMaxActionHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAXACTION_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend class ASoldierCharacter;

	UCombatComponent();

	void EquipWeapon(AWeapon *WeaponToEquip);

	void FireButtonPressed(bool bPressed);

protected:
#pragma region Event Functions
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
#pragma endregion

	void SetAiming(bool newIsAiming);

	void UpdateWalkSpeed(float newValue);

	void TraceUnderCrosshair(FHitResult &HitResult);

	void SetHUDCrosshairs(float DeltaTime);

	void UpdateHitTarget();

	void UpdateADSProgress(float DeltaTime);

	void UpdateFOV();

#pragma region Replication related

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool newIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize &TraceHitResult);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize &TraceHitResult);

#pragma endregion

private:
	ASoldierCharacter *Character;
	AMaxActionPlayerController *PlayerController;
	AMaxActionHUD *HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon *EquippedWeapon;

	UPROPERTY(Replicated)
	bool bIsAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere)
	float AimingWalkSpeed = 400.0f;

	FVector HitTarget;

	float ADSProgress = 0.0f;
	float InitialFOV;
	float ADSTimeElapsed;
	FWeaponStatistics WeaponStats;

	// Firing
	float LastFireTime;
	bool bFiringButtonPressed;
	FTimerHandle FireTimer;

	void Fire();
	void StartFiring();
	void StopFiring();
};
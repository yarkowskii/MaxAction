// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	void EquipWeapon(AWeapon *WeaponToEquip);

	void FireButtonPressed(bool bPressed);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_EquippedWeapon();

	void SetAiming(bool newIsAiming);

	void UpdateWalkSpeed(float newValue);

	void TraceUnderCrosshair(FHitResult &HitResult);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool newIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize &TraceHitResult);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize &TraceHitResult);

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

	void SetHUDCrosshairs(float DeltaTime);

	void UpdateHitTarget();
};

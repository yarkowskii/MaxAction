// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MaxAction/Enums/TurningInPlace.h"
#include "SoldierCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class USpringArmComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class MAXACTION_API ASoldierCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *LookAction;

	/** Equip Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *EquipAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *CrouchAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *AimAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *FireAction;

	/** Input Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *GroundMappingContext;

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent *Combat;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon *OverlappingWeapon;

	UPROPERTY(EditAnywhere, Category = Animation)
	class UAnimMontage *FireMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage *HitReactMontage;

	UPROPERTY(EditAnywhere)
	float HideCameraDistanceThreshold = 100.0f;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon *OldWeapon);

public:
	ASoldierCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void OnRep_ReplicatedMovement() override;

	void SetOverlappingWeapon(AWeapon *Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE UCameraComponent *GetFollowCamera() const { return FirstPersonCameraComponent; }
	FORCEINLINE bool GetShouldRotateRootBone() const { return ShouldRotateRootBone; }

	FVector GetHitTarget() const;

	AWeapon *GetEquippedWeapon() const;
	void UpdateTurningInPlace(float DeltaTime);

	void PlayFireMontage(bool bAiming);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit();

	void HideCameraIfCharacterTooClose();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	virtual void Jump() override;

	void AimButtonPressed();
	void AimButtonReleased();

	void FireButtonPressed();
	void FireButtonReleased();

	void PlayHitReactMontage();

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	void UpdateAimOffsets(float DeltaTime);
	void SimProxiesTurn();
	void CalculatePitch();
	float CalculateSpeed();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	float TurnThreshold = 0.5f;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;

	FRotator LastAimRotation;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;

	ETurningInPlace TurningInPlace;

	bool ShouldRotateRootBone;
	
};

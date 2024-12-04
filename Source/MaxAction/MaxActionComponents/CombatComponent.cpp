// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "MaxAction/Weapon/Weapon.h"
#include "MaxAction/Character/SoldierCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include <Logging/LogMacros.h>

#define TRACE_LENGTH 80000.0f

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateWalkSpeed(BaseWalkSpeed);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bIsAiming);
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	if (bPressed)
	{
		FHitResult HitResult;
		TraceUnderCrosshair(HitResult);
		ServerFire(HitResult.ImpactPoint);
	}
}

void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize &TraceHitResult)
{
	MulticastFire(TraceHitResult);
}

void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize &TraceHitResult)
{
	if (EquippedWeapon)
	{
		Character->PlayFireMontage(bIsAiming);
		EquippedWeapon->Fire(TraceHitResult);
	}
}

void UCombatComponent::SetAiming(bool newIsAiming)
{
	bIsAiming = newIsAiming;
	ServerSetAiming(newIsAiming);
	UpdateWalkSpeed(newIsAiming ? AimingWalkSpeed : BaseWalkSpeed);
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (Character && EquippedWeapon)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::UpdateWalkSpeed(float newValue)
{
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = newValue;
	}
}

void UCombatComponent::TraceUnderCrosshair(FHitResult &HitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	if (UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection))
	{
		const FVector Start = CrosshairWorldPosition;
		const FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool newIsAiming)
{
	bIsAiming = newIsAiming;
	UpdateWalkSpeed(newIsAiming ? AimingWalkSpeed : BaseWalkSpeed);
}

void UCombatComponent::EquipWeapon(AWeapon *WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr)
		return;

	EquippedWeapon = WeaponToEquip;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket *HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);

	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

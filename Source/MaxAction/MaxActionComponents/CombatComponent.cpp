// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "MaxAction/Weapon/Weapon.h"
#include "MaxAction/Character/SoldierCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateWalkSpeed(BaseWalkSpeed);
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

void UCombatComponent::ServerSetAiming_Implementation(bool newIsAiming)
{
	bIsAiming = newIsAiming;
	UpdateWalkSpeed(newIsAiming ? AimingWalkSpeed : BaseWalkSpeed);
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

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	if (EquippedWeapon && bPressed)
	{
		Character->PlayFireMontage(bIsAiming);
		EquippedWeapon->Fire();
	}
}

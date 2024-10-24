// Fill out your copyright notice in the Description page of Project Settings.

#include "CharAnimInstance.h"
#include "SoldierCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    TryGetCharacter();
}

void UCharAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (SoldierCharacter == nullptr)
    {
        TryGetCharacter();
    }

    if (SoldierCharacter == nullptr)
    {
        return;
    }

    Velocity = SoldierCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    IsInAir = SoldierCharacter->GetCharacterMovement()->IsFalling();

    IsAccelerating = SoldierCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

    bIsCrouched = SoldierCharacter->bIsCrouched;
    bIsEquipped = SoldierCharacter->IsWeaponEquipped();
    bIsAiming = SoldierCharacter->IsAiming();
}

void UCharAnimInstance::TryGetCharacter()
{
    SoldierCharacter = Cast<ASoldierCharacter>(TryGetPawnOwner());
}

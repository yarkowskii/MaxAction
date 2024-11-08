// Fill out your copyright notice in the Description page of Project Settings.

#include "CharAnimInstance.h"
#include "SoldierCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MaxAction/Weapon/Weapon.h"

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
    EquippedWeapon = SoldierCharacter->GetEquippedWeapon();
    bIsAiming = SoldierCharacter->IsAiming();
    TurningInPlace = SoldierCharacter->GetTurningInPlace();
    
    FRotator AimRotation = SoldierCharacter->GetBaseAimRotation();
    FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(SoldierCharacter->GetVelocity());
    FRotator YawDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
    CurrentYawRotation = FMath::RInterpTo(CurrentYawRotation, YawDeltaRotation, DeltaTime, 6.f);
    YawOffset = CurrentYawRotation.Yaw;

    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation = SoldierCharacter->GetActorRotation();
    const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
    const float Target = Delta.Yaw / DeltaTime;
    const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
    Lean = FMath::Clamp(Interp, -90.f, 90.f);

    AO_Yaw = SoldierCharacter->GetAO_Yaw();
    AO_Pitch = SoldierCharacter->GetAO_Pitch();

    if (bIsEquipped && EquippedWeapon && EquippedWeapon->GetWeaponModel() && SoldierCharacter->GetMesh())
    {
        LeftHandTransform = EquippedWeapon->GetWeaponModel()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
        FVector OutPosition;
        FRotator OutRotation;
        SoldierCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
        LeftHandTransform.SetLocation(OutPosition);
        LeftHandTransform.SetRotation(FQuat(OutRotation));
    }
}

void UCharAnimInstance::TryGetCharacter()
{
    SoldierCharacter = Cast<ASoldierCharacter>(TryGetPawnOwner());
}

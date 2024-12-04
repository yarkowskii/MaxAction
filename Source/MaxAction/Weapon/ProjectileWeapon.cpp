// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileWeapon::Fire(const FVector &HitTarget)
{
    Super::Fire(HitTarget);

    if (!HasAuthority())
        return;

    APawn *InstigatorPawn = Cast<APawn>(GetOwner());

    const USkeletalMeshSocket *muzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

    if (muzzleFlashSocket)
    {
        FTransform socketTransform = muzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

        FRotator toTargetRotation = (HitTarget - socketTransform.GetLocation()).Rotation();

        if (ProjectileClass && InstigatorPawn)
        {
            FActorSpawnParameters spawnParams;
            spawnParams.Owner = GetOwner();
            spawnParams.Instigator = InstigatorPawn;

            UWorld *World = GetWorld();
            if (World)
            {
                World->SpawnActor<AProjectile>(
                    ProjectileClass,
                    socketTransform.GetLocation(),
                    toTargetRotation,
                    spawnParams);
            }
        }
    }
}

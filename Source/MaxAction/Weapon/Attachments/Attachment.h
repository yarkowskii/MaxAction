#pragma once

#include "CoreMinimal.h"
#include "MaxAction/Weapon/WeaponStatistics.h"
#include "Attachment.generated.h"

UCLASS()
class MAXACTION_API AAttachment : public AActor
{
    GENERATED_BODY()

public:
    AAttachment();

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    class USkeletalMeshComponent *Mesh;

    UPROPERTY(EditAnywhere, Category = "Stats")
    FWeaponStatistics WeaponStats;

    virtual void ModifyWeaponStats(FWeaponStatistics &OutStats) const;
};
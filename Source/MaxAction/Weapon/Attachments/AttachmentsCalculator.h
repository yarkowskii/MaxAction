#pragma once

#include "CoreMinimal.h"
#include "MaxAction/Weapon/WeaponStatistics.h"

class MAXACTION_API AttachmentsCalculator
{
public:
    static FWeaponStatistics CalculateWeaponStats(
        const FWeaponStatistics &BaseStats,
        const TArray<TSubclassOf<class AAttachment>> &Attachments);
};
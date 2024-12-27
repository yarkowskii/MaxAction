#include "ScopeAttachment.h"
#include "../WeaponStatistics.h"
#include <Logging/LogMacros.h>

void AScopeAttachment::ModifyWeaponStats(FWeaponStatistics &OutStats) const
{

    if (WeaponStats.ADSZoomMultiplier > .0f)
    {
        OutStats.ADSZoomMultiplier = WeaponStats.ADSZoomMultiplier;
    }

    if (WeaponStats.ADSSpeed > .0f)
    {
        OutStats.ADSSpeed = OutStats.ADSSpeed * WeaponStats.ADSSpeed;
    }

    // UE_LOG(LogTemp, Log, TEXT("Modified stats: %f"), WeaponStats.ADSZoomMultiplier);
}

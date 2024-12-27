
#include "Attachment.h"

AAttachment::AAttachment()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AAttachment::ModifyWeaponStats(FWeaponStatistics &OutStats) const
{
}

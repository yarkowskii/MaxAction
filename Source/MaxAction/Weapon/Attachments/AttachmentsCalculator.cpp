#include "AttachmentsCalculator.h"
#include "MaxAction/Weapon/WeaponStatistics.h"
#include "MaxAction/Weapon/Attachments/Attachment.h"

FWeaponStatistics AttachmentsCalculator::CalculateWeaponStats(
    const FWeaponStatistics &BaseStats,
    const TArray<TSubclassOf<class AAttachment>> &Attachments)
{
    // Create a new stats object with base values
    FWeaponStatistics ModifiedStats = BaseStats; // Creates a copy of BaseStats

    // If no attachments, return the base stats
    if (Attachments.Num() == 0)
    {
        return ModifiedStats;
    }

    // For each attachment, apply its modifications
    for (const TSubclassOf<AAttachment> &Attachment : Attachments)
    {
        if (Attachment)
        {
            // Here you'll modify ModifiedStats based on each attachment
            // Example (you'll implement these methods later):
            // Attachment->ModifyWeaponStats(ModifiedStats);
        }
    }

    return ModifiedStats;
}
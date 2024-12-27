#pragma once

#include "CoreMinimal.h"
#include "Attachment.h"
#include "ScopeAttachment.generated.h"

UCLASS()
class MAXACTION_API AScopeAttachment : public AAttachment
{
    GENERATED_BODY()

public:
    virtual void ModifyWeaponStats(FWeaponStatistics &OutStats) const override;
};
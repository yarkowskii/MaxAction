// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"
#include "MaxAction/Character/SoldierCharacter.h"

void AProjectileBullet::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{

    ASoldierCharacter *SoldierCharacter = Cast<ASoldierCharacter>(OtherActor);
    if(SoldierCharacter)
    {
        AController* OwnerController = SoldierCharacter->Controller;
        if(OwnerController)
        {
            UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
        }
    }


    Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
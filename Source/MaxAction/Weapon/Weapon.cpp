// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponModel"));
	WeaponModel->SetupAttachment(RootComponent);

	WeaponModel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponModel->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "MaxAction/Character/SoldierCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Casing.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MaxAction/HUD/MaxActionHUD.h"
#include "MaxAction/Weapon/Attachments/AttachmentsCalculator.h"
#include "MaxAction/Weapon/Attachments/Attachment.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

FHUDPackage AWeapon::CreateHUDPackage()
{
	FHUDPackage HUDPackage;
	HUDPackage.CrosshairsCenter = CrosshairsCenter;
	HUDPackage.CrosshairsLeft = CrosshairsLeft;
	HUDPackage.CrosshairsRight = CrosshairsRight;
	HUDPackage.CrosshairsTop = CrosshairsTop;
	HUDPackage.CrosshairsBottom = CrosshairsBottom;

	return HUDPackage;
}

FWeaponStatistics AWeapon::GetWeaponStats()
{
	// Use the calculator to apply attachment modifications
	return AttachmentsCalculator::CalculateWeaponStats(InitialStats, Attachments);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}

void AWeapon::TogglePickupWidget(bool bIsVisible)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bIsVisible);
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;

	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		TogglePickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::Fire(const FVector &HitTarget)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	if (CasingClass)
	{
		const USkeletalMeshSocket *casingEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject"));

		if (casingEjectSocket)
		{
			FTransform socketTransform = casingEjectSocket->GetSocketTransform(WeaponMesh);

			UWorld *world = GetWorld();
			if (world)
			{
				world->SpawnActor<ACasing>(
					CasingClass,
					socketTransform.GetLocation(),
					socketTransform.GetRotation().Rotator());
			}
		}
	}
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	ASoldierCharacter *SoldierCharacter = Cast<ASoldierCharacter>(OtherActor);
	if (SoldierCharacter)
	{
		SoldierCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	ASoldierCharacter *SoldierCharacter = Cast<ASoldierCharacter>(OtherActor);
	if (SoldierCharacter)
	{
		SoldierCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		TogglePickupWidget(false);
		break;
	}
}

void AWeapon::ServerAddAttachment_Implementation(TSubclassOf<AAttachment> NewAttachment)
{
	if (NewAttachment && !Attachments.Contains(NewAttachment))
	{
		Attachments.Add(NewAttachment);
		// Optionally notify clients or update weapon stats
	}
}

void AWeapon::ServerRemoveAttachment_Implementation(TSubclassOf<AAttachment> AttachmentToRemove)
{
	if (AttachmentToRemove)
	{
		Attachments.Remove(AttachmentToRemove);
		// Optionally notify clients or update weapon stats
	}
}

void AWeapon::ServerClearAttachments_Implementation()
{
	Attachments.Empty();
	// Optionally notify clients or update weapon stats
}

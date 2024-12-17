// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaxAction/HUD/MaxActionHUD.h"
#include "MaxAction/Weapon/WeaponStatistics.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class MAXACTION_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	FWeaponStatistics InitialStats;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere)
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent *WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	class USphereComponent *AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	class UWidgetComponent *PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimationAsset *FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	class UTexture2D *CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D *CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D *CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D *CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D *CrosshairsBottom;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	void TogglePickupWidget(bool bIsVisible);
	void SetWeaponState(EWeaponState State);

	virtual void Fire(const FVector &HitTarget);

	FORCEINLINE USkeletalMeshComponent *GetWeaponMesh() const { return WeaponMesh; }

	FHUDPackage CreateHUDPackage();

	FWeaponStatistics GetWeaponStats();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_WeaponState();
};

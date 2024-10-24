// Fill out your copyright notice in the Description page of Project Settings.

#include "SoldierCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MaxAction/MaxActionComponents/CombatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MaxAction/Weapon/Weapon.h"
#include "Net/UnrealNetwork.h"
#include "Engine/GameViewportClient.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Sets default values
ASoldierCharacter::ASoldierCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = false;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FirstPersonCameraComponent->bUsePawnControlRotation = false;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	bUseControllerRotationYaw = false;

	// Configure the CharacterMovement component
	if (UCharacterMovementComponent *CharMovement = GetCharacterMovement())
	{
		CharMovement->bOrientRotationToMovement = false;
		CharMovement->bUseControllerDesiredRotation = false;
		CharMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	}
}

// Called when the game starts or when spawned
void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoldierCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASoldierCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ASoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->Character = this;
	}
}

void ASoldierCharacter::OnRep_OverlappingWeapon(AWeapon *OldWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->TogglePickupWidget(true);
	}
	if (OldWeapon)
	{
		OldWeapon->TogglePickupWidget(false);
	}
}

void ASoldierCharacter::SetOverlappingWeapon(AWeapon *Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->TogglePickupWidget(false);
	}

	OverlappingWeapon = Weapon;

	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->TogglePickupWidget(true);
		}
	}
}

bool ASoldierCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

void ASoldierCharacter::Move(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// Get the camera's forward and right vectors
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement input
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// Rotate character to face movement direction if moving
		// if (!MovementVector.IsNearlyZero())
		// {
		// 	FVector MovementDirection = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X).GetSafeNormal();
		// 	FRotator TargetRotation = MovementDirection.Rotation();
		// 	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
		// 	SetActorRotation(NewRotation);
		// }
	}
}

void ASoldierCharacter::Look(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASoldierCharacter::EquipButtonPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("%s: EquipButtonPressed called"),
		   HasAuthority() ? TEXT("Server") : TEXT("Client"));

	if (Combat)
	{
		ServerEquipButtonPressed();
	}
}

void ASoldierCharacter::ServerEquipButtonPressed_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server: ServerEquipButtonPressed called"));
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

// Called to bind functionality to input
void ASoldierCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Enhanced Input Mapping Context
	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(GroundMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoldierCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoldierCharacter::Look);

		// Equip
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASoldierCharacter::EquipButtonPressed);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class MAXACTION_API ACasing : public AActor
{
	GENERATED_BODY()

public:
	ACasing();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent *CasingMesh;

	UPROPERTY(EditAnywhere, Category = "Casing")
	float EjectionForce;

protected:
	virtual void BeginPlay() override;
};

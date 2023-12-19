// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptPowerupActor.h"
#include "BuptHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API ABuptHealthPotion : public ABuptPowerupActor
{
	GENERATED_BODY()

public:

	ABuptHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	float HealAmount;

	UPROPERTY(EditAnywhere)
	int32 CreditCost;
};

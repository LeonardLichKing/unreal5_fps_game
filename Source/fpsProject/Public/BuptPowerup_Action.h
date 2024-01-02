// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptPowerupActor.h"
#include "BuptPowerup_Action.generated.h"

class UBuptAction;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API ABuptPowerup_Action : public ABuptPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<UBuptAction> ActionToGrant;

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};

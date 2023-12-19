// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptPowerupActor.h"
#include "BuptCredits.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API ABuptCredits : public ABuptPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;

public:
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	ABuptCredits();
};

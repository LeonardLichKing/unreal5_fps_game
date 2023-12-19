// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptCredits.h"

#include "BuptPlayerState.h"


ABuptCredits::ABuptCredits()
{
	CreditsAmount = 80;
}

void ABuptCredits::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (ABuptPlayerState* PS = InstigatorPawn->GetPlayerState<ABuptPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}

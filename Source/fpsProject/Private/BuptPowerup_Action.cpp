// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptPowerup_Action.h"

#include "BuptAction.h"
#include "BuptActionrComponent.h"

void ABuptPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	// Make sure we have instigator & that action class was set up
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	UBuptActionrComponent* ActionComp = Cast<UBuptActionrComponent>(InstigatorPawn->GetComponentByClass(UBuptActionrComponent::StaticClass()));
	// Check if Player already has action class
	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			//UE_LOG(LogTemp, Log, TEXT("Instigator already has action of class: %s"), *GetNameSafe(ActionToGrant));
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		// Give new Ability
		ActionComp->AddAction(InstigatorPawn, ActionToGrant);	
		HideAndCooldownPowerup();
	}
}

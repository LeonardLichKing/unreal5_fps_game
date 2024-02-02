// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptHealthPotion.h"

#include "BuptAttributeComponent.h"
#include "BuptPlayerState.h"
#define LOCTEXT_NAMESPACE "InteractableActors"

ABuptHealthPotion::ABuptHealthPotion()
{
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	HealAmount=20.0f;
	CreditCost=50;
}

void ABuptHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	UBuptAttributeComponent* AttributeComp=UBuptAttributeComponent::GetAttributes(InstigatorPawn);
	if(ensure(AttributeComp)&&AttributeComp->IsInjured())
	{
		if(ABuptPlayerState* PS=InstigatorPawn->GetPlayerState<ABuptPlayerState>())
		{
			if(PS->RemoveCredits(CreditCost)&&AttributeComp->ApplyHealthChange(this,HealAmount))
			{
				HideAndCooldownPowerup();
			}
		}
		
		// bool IsApplyHeal=AttributeComp->ApplyHealthChange(this,HealAmount);
		// if(IsApplyHeal)
		// {
		// 	HideAndCooldownPowerup();
		// }
	}
}

FText ABuptHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UBuptAttributeComponent* AttributeComp=UBuptAttributeComponent::GetAttributes(InstigatorPawn);
	if(AttributeComp&&!AttributeComp->IsInjured())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning","Already at full health.");
	}
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage","Cost {0} Credits, Restore {1} health."),CreditCost,HealAmount);
}

#undef LOCTEXT_NAMESPACE
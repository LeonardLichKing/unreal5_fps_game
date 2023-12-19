// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptHealthPotion.h"

#include "BuptAttributeComponent.h"
#include "BuptPlayerState.h"

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

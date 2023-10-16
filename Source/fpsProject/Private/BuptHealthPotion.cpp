// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptHealthPotion.h"

#include "BuptAttributeComponent.h"

ABuptHealthPotion::ABuptHealthPotion()
{
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	HealAmount=20.0f;
}

void ABuptHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	UBuptAttributeComponent* AttributeComp=Cast<UBuptAttributeComponent>(InstigatorPawn->GetComponentByClass(UBuptAttributeComponent::StaticClass()));
	if(ensure(AttributeComp)&&AttributeComp->IsInjured())
	{
		bool IsApplyHeal=AttributeComp->ApplyHealthChange(HealAmount);
		if(IsApplyHeal)
		{
			HideAndCooldownPowerup();
		}
	}
}

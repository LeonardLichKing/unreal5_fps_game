// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptEffect_Thorns.h"

#include "BuptActionrComponent.h"
#include "BuptAttributeComponent.h"
#include "BuptGameplayFunctionLibrary.h"

UBuptEffect_Thorns::UBuptEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UBuptEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UBuptAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	// Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Return damage sender...
		UBuptGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}

void UBuptEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Start listening
	UBuptAttributeComponent* Attributes = UBuptAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UBuptEffect_Thorns::OnHealthChanged);
	}
}

void UBuptEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UBuptAttributeComponent* Attributes = UBuptAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UBuptEffect_Thorns::OnHealthChanged);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAttributeComponent.h"


// Sets default values for this component's properties
UBuptAttributeComponent::UBuptAttributeComponent()
{
	HealthMax=100;
	Health = 100;
}


bool UBuptAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor,-GetMaxHealth());
}

bool UBuptAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	float OldHealth=Health;
	
	Health=FMath::Clamp(Health+Delta,0,HealthMax);

	float ActualDelta=Health-OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return true;
}

bool UBuptAttributeComponent::IsAlive() const
{
	return Health>0.0f;
}

bool UBuptAttributeComponent::IsInjured() const
{
	return Health<HealthMax;
}

float UBuptAttributeComponent::GetHealth() const
{
	return Health;
}

float UBuptAttributeComponent::GetMaxHealth() const
{
	return HealthMax;
}

UBuptAttributeComponent* UBuptAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<UBuptAttributeComponent>(FromActor->GetComponentByClass(UBuptAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UBuptAttributeComponent::IsActorAlive(AActor* Actor)
{
	UBuptAttributeComponent* AttributeComp=GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}



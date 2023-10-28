// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAttributeComponent.h"

// Sets default values for this component's properties
UBuptAttributeComponent::UBuptAttributeComponent()
{
	HealthMax=100;
	Health = 100;
}


bool UBuptAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth=Health;
	
	Health=FMath::Clamp(Health+Delta,0,HealthMax);

	float ActualDelta=Health-OldHealth;

	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

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





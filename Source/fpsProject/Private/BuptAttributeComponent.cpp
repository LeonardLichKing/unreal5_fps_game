// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAttributeComponent.h"

// Sets default values for this component's properties
UBuptAttributeComponent::UBuptAttributeComponent()
{
	Health = 100;
}


bool UBuptAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}





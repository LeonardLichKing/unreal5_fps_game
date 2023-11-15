// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAction.h"

void UBuptAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running: %s"),*GetNameSafe(this));
}

void UBuptAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stopped: %s"),*GetNameSafe(this));
}

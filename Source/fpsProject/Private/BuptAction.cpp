// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAction.h"

#include "BuptActionrComponent.h"

void UBuptAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running: %s"),*GetNameSafe(this));
}

void UBuptAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stopped: %s"),*GetNameSafe(this));
}

UWorld* UBuptAction::GetWorld() const
{
	//outer is set when creating actions when call NewObject<T>
	UBuptActionrComponent* Comp=Cast<UBuptActionrComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

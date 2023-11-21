// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAction.h"

#include "BuptActionrComponent.h"

void UBuptAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running: %s"),*GetNameSafe(this));

	UBuptActionrComponent* Comp=GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning=true;
}

void UBuptAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stopped: %s"),*GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	UBuptActionrComponent* Comp=GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning=false;
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

UBuptActionrComponent* UBuptAction::GetOwningComponent() const
{
	return Cast<UBuptActionrComponent>(GetOuter());
}

bool UBuptAction::IsRunning() const
{
	return bIsRunning;
}

bool UBuptAction::CanStart_Implementation(AActor* Instigator)
{
	if(bIsRunning)
	{
		return false;
	}
	
	UBuptActionrComponent* Comp=GetOwningComponent();
	
	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

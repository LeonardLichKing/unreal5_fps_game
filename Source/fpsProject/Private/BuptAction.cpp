// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAction.h"

#include "BuptActionrComponent.h"
#include "fpsProject/fpsProject.h"
#include "Net/UnrealNetwork.h"

void UBuptAction::Initialize(UBuptActionrComponent* NewActionComp)
{
	ActionComp=NewActionComp;
}

void UBuptAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running: %s"),*GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UBuptActionrComponent* Comp=GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);

	RepData.bIsRunning=true;
	RepData.Instigator=Instigator;

	TimeStarted=GetWorld()->TimeSeconds;
	
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(),this);
}

void UBuptAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stopped: %s"),*GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	//ensureAlways(bIsRunning);
	
	UBuptActionrComponent* Comp=GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);

	RepData.bIsRunning=false;
	RepData.Instigator=Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(),this);
}

UWorld* UBuptAction::GetWorld() const
{
	//outer is set when creating actions when call NewObject<T>
	//Outer被设置成了拥有该action的player
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

UBuptActionrComponent* UBuptAction::GetOwningComponent() const
{
	return ActionComp;
}

void UBuptAction::OnRep_RepData()
{
	if(RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool UBuptAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool UBuptAction::CanStart_Implementation(AActor* Instigator)
{
	if(RepData.bIsRunning)
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

void UBuptAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuptAction,RepData);
	DOREPLIFETIME(UBuptAction,ActionComp);
}
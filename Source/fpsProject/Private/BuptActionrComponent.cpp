// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptActionrComponent.h"

#include "BuptAction.h"
#include "Engine/ActorChannel.h"
#include "fpsProject/fpsProject.h"
#include "Net/UnrealNetwork.h"

UBuptActionrComponent::UBuptActionrComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UBuptActionrComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		for(TSubclassOf<UBuptAction> ActionClass:DefaultActions)
		{
			AddAction(GetOwner(),ActionClass);
		}
	}
	
}

void UBuptActionrComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// FString DebugMsg=GetNameSafe(GetOwner())+" : "+ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::White,DebugMsg);

	// Draw All Actions
	for (UBuptAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
	
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UBuptActionrComponent::AddAction(AActor* Instigator,TSubclassOf<UBuptAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	UBuptAction* NewAction=NewObject<UBuptAction>(GetOwner(),ActionClass);
	if(ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);

		if(NewAction->bAutoStart&&ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
	
}

void UBuptActionrComponent::RemoveAction(UBuptAction* ActionToRemove)
{
	if(!ensure(ActionToRemove&&!ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

UBuptAction* UBuptActionrComponent::GetAction(TSubclassOf<UBuptAction> ActionClass) const
{
	for (UBuptAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

bool UBuptActionrComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(UBuptAction* Action:Actions)
	{
		if(Action&&Action->ActionName==ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString FailedMsg=FString::Printf(TEXT("Failed to run: %s"),*ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,FailedMsg);
				continue;
			}

			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator,ActionName);
			}
			
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UBuptActionrComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(UBuptAction* Action:Actions)
	{
		if(Action&&Action->ActionName==ActionName)
		{
			if(Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void UBuptActionrComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator,ActionName);
}

bool UBuptActionrComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UBuptAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UBuptActionrComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuptActionrComponent,Actions);
}
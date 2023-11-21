// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptActionrComponent.h"

#include "BuptAction.h"

UBuptActionrComponent::UBuptActionrComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuptActionrComponent::BeginPlay()
{
	Super::BeginPlay();
	for(TSubclassOf<UBuptAction> ActionClass:DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UBuptActionrComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString DebugMsg=GetNameSafe(GetOwner())+" : "+ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::White,DebugMsg);
}

void UBuptActionrComponent::AddAction(TSubclassOf<UBuptAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	UBuptAction* NewAction=NewObject<UBuptAction>(this,ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);		
	}
	
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

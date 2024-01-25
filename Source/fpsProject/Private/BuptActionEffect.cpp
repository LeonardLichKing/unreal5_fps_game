// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptActionEffect.h"
#include "BuptActionrComponent.h"

UBuptActionEffect::UBuptActionEffect()
{
	bAutoStart=true;
}

void UBuptActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}

void UBuptActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(Duration>0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"StopAction",Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle,Delegate,Duration,false);
	}

	if(Period>0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"ExecutePeriodicEffect",Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle,Delegate,Period,true);
	}
}

void UBuptActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle)<KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	UBuptActionrComponent* Comp=GetOwningComponent();
	if(Comp)
	{
		Comp->RemoveAction(this);
	}
}

float UBuptActionEffect::GetTimeRemaining() const
{
	float EndTime=TimeStarted+Duration;
	return EndTime-GetWorld()->TimeSeconds;
}

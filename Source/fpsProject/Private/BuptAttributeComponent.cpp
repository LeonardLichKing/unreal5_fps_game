// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAttributeComponent.h"

#include "BuptGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"),1.0f,TEXT("Global Damage Modifier for Attribute Component."),ECVF_Cheat);

// Sets default values for this component's properties
UBuptAttributeComponent::UBuptAttributeComponent()
{
	HealthMax=100;
	Health = 100;

	Rage=0;
	RageMax=100;

	SetIsReplicatedByDefault(true);
}

bool UBuptAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor,-GetMaxHealth());
}

bool UBuptAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if(!GetOwner()->CanBeDamaged()&&Delta<0)
	{
		return false;
	}

	if(Delta<0.0f)
	{
		float DamageMultiplier=CVarDamageMultiplier.GetValueOnGameThread();

		Delta*=DamageMultiplier;
	}
	
	float OldHealth=Health;
	
	Health=FMath::Clamp(Health+Delta,0,HealthMax);

	float ActualDelta=Health-OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if(ActualDelta!=0.0f)
	{
		MulticastHealthChanged(InstigatorActor,Health,ActualDelta);
	}

	//The AttributeComp's Owner Died
	if(ActualDelta<0.0f&&Health==0.0f)
	{
		ABuptGameModeBase* GM=GetWorld()->GetAuthGameMode<ABuptGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(),InstigatorActor);
		}
	}

	return true;
}

bool UBuptAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0;
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

float UBuptAttributeComponent::GetRage() const
{
	return Rage;
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

void UBuptAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
	float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this,NewHealth,Delta);
}

void UBuptAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuptAttributeComponent,Health);
	DOREPLIFETIME(UBuptAttributeComponent,HealthMax);
	// DOREPLIFETIME_CONDITION(UBuptAttributeComponent,HealthMax,COND_OwnerOnly);
}


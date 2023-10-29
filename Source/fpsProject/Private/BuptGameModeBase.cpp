// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptGameModeBase.h"

#include "BuptAttributeComponent.h"
#include "EngineUtils.h"
#include "SAdvancedRotationInputBox.h"
#include "AI/BuptAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ABuptGameModeBase::ABuptGameModeBase()
{
	SpawnTimerInterval=2.0f;
}

void ABuptGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ABuptGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);
}

void ABuptGameModeBase::SpawnBotTimerElapsed()
{
	int32 NrOfAliveBots=0;
	for(TActorIterator<ABuptAICharacter> It(GetWorld());It;++It)
	{
		ABuptAICharacter* Bot=*It;
		UBuptAttributeComponent* AttributeComp=UBuptAttributeComponent::GetAttributes(Bot);
		if(AttributeComp&&AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount=10.0f;

	if(DifficultyCurve)
	{
		MaxBotCount=DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	
	if(NrOfAliveBots>=MaxBotCount)
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance=UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ABuptGameModeBase::OnQueryCompleted);
	}
}

void ABuptGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus!=EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn bot EQS Query Failed"));
		return;
	}
	
	TArray<FVector> Locations=QueryInstance->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator,Params);
	}
}
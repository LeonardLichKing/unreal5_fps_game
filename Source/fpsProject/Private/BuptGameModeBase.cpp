// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptGameModeBase.h"

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
		return;
	}
	
	TArray<FVector> Locations=QueryInstance->GetResultsAsLocations();
}
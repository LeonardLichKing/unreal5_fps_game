// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "BuptGameModeBase.generated.h"

class UEnvQuery;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API ABuptGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	void SpawnBotTimerElapsed();
public:
	ABuptGameModeBase();
	
	virtual void StartPlay() override;
};

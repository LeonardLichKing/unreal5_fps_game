// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "BuptGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
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
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	
public:
	ABuptGameModeBase();
	
	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);
	
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();
};


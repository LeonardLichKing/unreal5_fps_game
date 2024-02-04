// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "BuptGameModeBase.generated.h"

class UBuptMonsterData;
class UDataTable;
class UBuptSaveGame;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FMonsterInfoRow:public FTableRowBase
{
	GENERATED_BODY()
public:

	FMonsterInfoRow()
	{
		Weight=1.0f;
		SpawnCost=5.0f;
		KillReward=20.0f;
	}

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	
	// UPROPERTY(EditAnywhere,BlueprintReadOnly)
	// TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float SpawnCost;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float KillReward;
};
/**
 * 
 */
UCLASS()
class FPSPROJECT_API ABuptGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:

	FString SlotName;

	UPROPERTY()
	UBuptSaveGame* CurrentSaveGame;
	
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UDataTable* MonsterTable;
	
	// UPROPERTY(EditDefaultsOnly,Category="AI")
	// TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TObjectPtr<UEnvQuery> PowerupSpawnQuery;

	/* All power-up classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	/* Distance required between power-up spawn locations */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance;

	/* Amount of powerups to spawn during match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount;

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnMonsterLoaded(FPrimaryAssetId LoadedId,FVector SpawnLocation);
	
	void OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	
public:
	ABuptGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);
	
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable,Category="Save Game")
	void WriteSaveGame();

	void LoadSaveGame();
};


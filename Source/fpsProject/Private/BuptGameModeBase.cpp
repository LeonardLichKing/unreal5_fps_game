// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptGameModeBase.h"

#include "BuptAttributeComponent.h"
#include "BuptCharacter.h"
#include "BuptPlayerState.h"
#include "EngineUtils.h"
#include "SAdvancedRotationInputBox.h"
#include "AI/BuptAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);

ABuptGameModeBase::ABuptGameModeBase()
{
	SpawnTimerInterval=2.0f;
	CreditsPerKill=20;

	DesiredPowerupCount=10;
	RequiredPowerupDistance=2000;

	PlayerStateClass=ABuptPlayerState::StaticClass();
}

void ABuptGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ABuptGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp,Log,TEXT("OnActorKilled: VictimActor: %s,Killer: %s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));
	ABuptCharacter* Player=Cast<ABuptCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());

		float RespawnDelay=2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}

	APawn* KillerPawn=Cast<APawn>(Killer);
	if(KillerPawn)
	{
		ABuptPlayerState* PS=KillerPawn->GetPlayerState<ABuptPlayerState>();
		if(PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}

void ABuptGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ABuptGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);

	if (ensure(PowerupClasses.Num() > 0))
	{
		// Run EQS to find potential power-up spawn locations
		//UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		//if (ensure(QueryInstance))
		//{
		//	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
		//}

		// Skip the Blueprint wrapper and use the direct C++ option which the Wrapper uses as well
		FEnvQueryRequest Request(PowerupSpawnQuery, this);
		Request.Execute(EEnvQueryRunMode::AllMatching, this, &ABuptGameModeBase::OnPowerupSpawnQueryCompleted);
	}
	
}

void ABuptGameModeBase::KillAll()
{
	for(TActorIterator<ABuptAICharacter> It(GetWorld());It;++It)
	{
		ABuptAICharacter* Bot=*It;
		UBuptAttributeComponent* AttributeComp=UBuptAttributeComponent::GetAttributes(Bot);
		if(AttributeComp&&AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);//TODO:maybe switch "this" to Player for kill credits is better
		}
	}
}

void ABuptGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ABuptGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ABuptGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
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

void ABuptGameModeBase::OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	// Retrieve all possible locations that passed the query
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

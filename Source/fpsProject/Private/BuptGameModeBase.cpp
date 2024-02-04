// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptGameModeBase.h"

#include "BuptActionrComponent.h"
#include "BuptAttributeComponent.h"
#include "BuptCharacter.h"
#include "BuptGamePlayInterface.h"
#include "BuptMonsterData.h"
#include "BuptPlayerState.h"
#include "BuptSaveGame.h"
#include "EngineUtils.h"
#include "SAdvancedRotationInputBox.h"
#include "AI/BuptAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "fpsProject/fpsProject.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GamePlayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);

ABuptGameModeBase::ABuptGameModeBase()
{
	SpawnTimerInterval=2.0f;
	CreditsPerKill=20;

	DesiredPowerupCount=5;
	RequiredPowerupDistance=2000;

	PlayerStateClass=ABuptPlayerState::StaticClass();
	SlotName="SaveGame01";
}

void ABuptGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void ABuptGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{

	ABuptPlayerState* PS=NewPlayer->GetPlayerState<ABuptPlayerState>();
	if(PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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
	if(KillerPawn&&KillerPawn!=VictimActor)
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
		if(MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("",Rows);

			//Get Random Enemy
			int32 RandomIndex=FMath::RandRange(0,Rows.Num()-1);
			FMonsterInfoRow* SeletedRow=Rows[RandomIndex];

			UAssetManager* Manager=UAssetManager::GetIfValid();
			if(Manager)
			{
				LogOnScreen(this, "Start loading.", FColor::Red);
				TArray<FName> Bundles;

				FStreamableDelegate Delegate=FStreamableDelegate::CreateUObject(this,&ABuptGameModeBase::OnMonsterLoaded,SeletedRow->MonsterId,Locations[0]);;
				
				Manager->LoadPrimaryAsset(SeletedRow->MonsterId,Bundles,Delegate);
			}
			
		}
	}
}

void ABuptGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	UAssetManager* Manager=UAssetManager::GetIfValid();
	if(Manager)
	{
		UBuptMonsterData* MonsterData=Cast<UBuptMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if(MonsterData)
		{
			//AlwaysSpawn±ØÐëÉèÖÃ
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* NewBot=GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass,SpawnLocation,FRotator::ZeroRotator,Params);
			if(NewBot)
			{
				LogOnScreen(this,FString::Printf(TEXT("Spawn enemy: %s (%s)"),*GetNameSafe(NewBot),*GetNameSafe(MonsterData)));
				UBuptActionrComponent* ActionComp=Cast<UBuptActionrComponent>(NewBot->GetComponentByClass(UBuptActionrComponent::StaticClass()));
				if(ActionComp)
				{
					for(TSubclassOf<UBuptAction>ActionClass:MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot,ActionClass);
					}
				}
			}
		}
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

void ABuptGameModeBase::WriteSaveGame()
{
	for(int32 i=0;i<GameState->PlayerArray.Num();i++)
	{
		ABuptPlayerState* PS=Cast<ABuptPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;//single player case
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for(FActorIterator It(GetWorld());It;++It)
	{
		AActor* Actor=*It;
		if(!Actor->Implements<UBuptGamePlayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName=Actor->GetName();
		ActorData.Transform=Actor->GetTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);
		Ar.ArIsSaveGame=true;
		
		Actor->Serialize(Ar);
		
		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void ABuptGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame=Cast<UBuptSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
		if(CurrentSaveGame==nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Failed To Load SaveGame Data."));
			return;
		}

		for(FActorIterator It(GetWorld());It;++It)
		{
			AActor* Actor=*It;
			if(!Actor->Implements<UBuptGamePlayInterface>())
			{
				continue;
			}

			for(FActorSaveData ActorData:CurrentSaveGame->SavedActors)
			{
				if(ActorData.ActorName==Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader,true);
					Ar.ArIsSaveGame=true;
					Actor->Serialize(Ar);
					IBuptGamePlayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}

		UE_LOG(LogTemp,Log,TEXT("Load SaveGame Data."));
	}
	else
	{
		CurrentSaveGame=Cast<UBuptSaveGame>(UGameplayStatics::CreateSaveGameObject(UBuptSaveGame::StaticClass()));
		UE_LOG(LogTemp,Log,TEXT("Create New SaveGame Data."));
	}
	
}
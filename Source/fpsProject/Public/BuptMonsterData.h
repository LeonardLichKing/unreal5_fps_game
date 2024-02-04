// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuptMonsterData.generated.h"

class UBuptAction;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBuptMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Spawn Info")
	TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Spawn Info")
	TArray<TSubclassOf<UBuptAction>> Actions;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	UTexture2D* Icon;

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Monsters",GetFName());
	}
};

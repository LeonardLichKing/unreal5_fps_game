// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BuptSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBuptSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	int32 Credits;
};

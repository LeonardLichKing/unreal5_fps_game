// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuptAction.generated.h"

class UWorld;
/**
 * 
 */
UCLASS(Blueprintable)
class FPSPROJECT_API UBuptAction : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StopAction(AActor* Instigator);
	
	//Action nickname to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly,Category="Action")
	FName ActionName;

	UWorld* GetWorld() const override;
};

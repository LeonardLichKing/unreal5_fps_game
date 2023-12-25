// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptAction.h"
#include "BuptActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBuptActionEffect : public UBuptAction
{
	GENERATED_BODY()

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
	
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent,Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:

	UBuptActionEffect();
};

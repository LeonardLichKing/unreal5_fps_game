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

	//duration means how many times it will continue
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	float Duration;

	//period means the working frequency
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent,Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:

	UBuptActionEffect();
};

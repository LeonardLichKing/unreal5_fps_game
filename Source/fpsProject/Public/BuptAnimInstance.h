// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BuptAnimInstance.generated.h"

class UBuptActionrComponent;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBuptAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly,Category="Animation")
	TObjectPtr<UBuptActionrComponent> ActionComp;

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BuptBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBuptBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector LowHealthKey;
	
	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin="0.0", ClampMax="1.0"))
	float LowHealthFraction;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:

	UBuptBTService_CheckHealth();
};

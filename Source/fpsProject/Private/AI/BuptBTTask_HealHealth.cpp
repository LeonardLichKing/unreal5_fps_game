// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BuptBTTask_HealHealth.h"

#include "AIController.h"
#include "BuptAttributeComponent.h"
#include "AI/BuptAICharacter.h"

EBTNodeResult::Type UBuptBTTask_HealHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController=OwnerComp.GetAIOwner();
	if(ensure(AIController))
	{
		ABuptAICharacter* AIPawn=Cast<ABuptAICharacter>(AIController->GetPawn());
		if(ensure(AIPawn))
		{
			UBuptAttributeComponent* AttributeComp=AIPawn->FindComponentByClass<UBuptAttributeComponent>();
			AttributeComp->ApplyHealthChange(AttributeComp->GetMaxHealth());
			return AttributeComp->IsInjured()?EBTNodeResult::Failed:EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

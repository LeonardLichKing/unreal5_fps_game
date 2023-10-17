// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BuptBTService_CheckHealth.h"

#include "AIController.h"
#include "BuptAttributeComponent.h"
#include "AI/BuptAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBuptBTService_CheckHealth::UBuptBTService_CheckHealth()
{
	LowHealthFraction=0.3f;
}

void UBuptBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp=OwnerComp.GetBlackboardComponent();
	if(ensure(BBComp))
	{
		AAIController* AIController=OwnerComp.GetAIOwner();
		if(ensure(AIController))
		{
			ABuptAICharacter* AIPawn=Cast<ABuptAICharacter>(AIController->GetPawn());
			if(ensure(AIPawn))
			{
				UBuptAttributeComponent* AttributeComp=AIPawn->FindComponentByClass<UBuptAttributeComponent>();
				bool bLowHealth=AttributeComp->GetHealth()/AttributeComp->GetMaxHealth()<=LowHealthFraction;
				BBComp->SetValueAsBool(LowHealthKey.SelectedKeyName,bLowHealth);
			}
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BuptBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBuptBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp=OwnerComp.GetBlackboardComponent();

	if(ensure(BlackboardComp))
	{
		AActor* TargetActor=Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			AAIController* MyController=OwnerComp.GetAIOwner();
			if(ensure(MyController))
			{
				APawn* AIPawn=MyController->GetPawn();
				if(ensure(AIPawn))
				{
					float DistanceTo=FVector::Distance(AIPawn->GetActorLocation(),TargetActor->GetActorLocation());
					bool bWithinRange=DistanceTo<2000.0f;

					bool bHasLOS=false;
					if(bWithinRange)
					{
						bHasLOS=MyController->LineOfSightTo(TargetActor);
					}
					
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName,bWithinRange&&bHasLOS);
				}
			}
		}
	}

	
	
}

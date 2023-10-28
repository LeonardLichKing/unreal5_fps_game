// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BuptAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BuptAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ABuptAICharacter::ABuptAICharacter()
{
	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp=CreateDefaultSubobject<UBuptAttributeComponent>("AttributeComp");

	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABuptAICharacter::OnHealthChanged(AActor* InstigatorActor, UBuptAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta<0)
	{
		if(NewHealth<=0.0f)
		{
			//stop behavior tree
			AAIController* AIC=Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

void ABuptAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ABuptAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this,&ABuptAICharacter::OnHealthChanged);
}
void ABuptAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC=Cast<AAIController>(GetController());
	if(AIC)
	{
		UBlackboardComponent* BBComp=AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor",Pawn);

		DrawDebugString(GetWorld(),GetActorLocation(),"PLAYER SPOTTED",nullptr,FColor::White,4.0f,true);
	}
}

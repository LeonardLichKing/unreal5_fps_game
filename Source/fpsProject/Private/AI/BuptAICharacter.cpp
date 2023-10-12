// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BuptAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ABuptAICharacter::ABuptAICharacter()
{
	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	
}

void ABuptAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ABuptAICharacter::OnPawnSeen);
	
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BuptAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BuptActionrComponent.h"
#include "BuptAttributeComponent.h"
#include "BuptWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ABuptAICharacter::ABuptAICharacter()
{
	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp=CreateDefaultSubobject<UBuptAttributeComponent>("AttributeComp");
	ActionComp=CreateDefaultSubobject<UBuptActionrComponent>("ActionComp");

	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABuptAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ABuptAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this,&ABuptAICharacter::OnHealthChanged);
}

void ABuptAICharacter::SetTargetActor(AActor* NewTarget)
{
	//TODO:it's necessary to ignore the teammate AIs
	AAIController* AIC=Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor",NewTarget);
	}
}

void ABuptAICharacter::OnHealthChanged(AActor* InstigatorActor, UBuptAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta<0)
	{
		if(InstigatorActor!=this)
		{
			SetTargetActor(InstigatorActor);
		}

		if(ActiveHealthBar==nullptr)
		{
			ActiveHealthBar=CreateWidget<UBuptWorldUserWidget>(GetWorld(),HealthBarWidgetClass);
		
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor=this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

void ABuptAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	UBuptWorldUserWidget* NewWidget = CreateWidget<UBuptWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;
		// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
		// May end up behind the minion health bar otherwise.
		NewWidget->AddToViewport(10);
	}
	DrawDebugString(GetWorld(),GetActorLocation(),"PLAYER SPOTTED",nullptr,FColor::White,4.0f,true);
}

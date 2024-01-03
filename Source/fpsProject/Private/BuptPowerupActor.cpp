// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptPowerupActor.h"

#include "MovieSceneSequenceID.h"
#include "Components/SphereComponent.h"

// Sets default values
ABuptPowerupActor::ABuptPowerupActor()
{
	SphereComp=CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent=SphereComp;

	// MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// MeshComp->SetupAttachment(RootComponent);

	RespawnTime=10.0f;

	SetReplicates(true);
}

void ABuptPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	IBuptGamePlayInterface::Interact_Implementation(InstigatorPawn);
}

void ABuptPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ABuptPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer,this,&ABuptPowerupActor::ShowPowerup,RespawnTime);
}

void ABuptPowerupActor::SetPowerupState(bool bIsActivate)
{
	SetActorEnableCollision(bIsActivate);

	RootComponent->SetVisibility(bIsActivate,true);
}




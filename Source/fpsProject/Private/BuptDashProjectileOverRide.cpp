// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptDashProjectileOverRide.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABuptDashProjectileOverRide::ABuptDashProjectileOverRide()
{
	TeleportDelay=0.2f;
	DetonateDelay=0.2f;

	MovementComp->InitialSpeed=6000.0f;
}


void ABuptDashProjectileOverRide::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate,this,&ABuptDashProjectileOverRide::Explode,DetonateDelay);
}

void ABuptDashProjectileOverRide::Explode_Implementation()
{
	//Super::Explode_Implementation();

	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport,this,&ABuptDashProjectileOverRide::TeleportInstigator,TeleportDelay);
}

void ABuptDashProjectileOverRide::TeleportInstigator()
{
	AActor* ActorToTeleport=GetInstigator();
	if(ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(),ActorToTeleport->GetActorRotation(),false,false);
	}

	//Destroy();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABuptExplosiveBarrel::ABuptExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ʵ��һ��ί�����ڰ󶨺���
	FScriptDelegate Delegater;
	Delegater.BindUFunction(this, STATIC_FUNCTION_FNAME(TEXT("BuptExplosiveBarrel::OnComponentHit")));

	//ʵ������̬���������
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->OnComponentHit.Add(Delegater);
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMeshComp;

	//ʵ�������������
	RadialComp = CreateDefaultSubobject<URadialForceComponent>("RadialComp");
	RadialComp->SetupAttachment(StaticMeshComp);
	RadialComp->Radius = 500.0f;
	RadialComp->ImpulseStrength = 2000.0f;
	RadialComp->bImpulseVelChange = true;
	RadialComp->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ABuptExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuptExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, FHitResult Hit)
{
	RadialComp->FireImpulse();
}

// Called every frame
void ABuptExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


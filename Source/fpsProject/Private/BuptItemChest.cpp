// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptItemChest.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABuptItemChest::ABuptItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;
}

void ABuptItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}

// Called when the game starts or when spawned
void ABuptItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuptItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABuptItemChest::ABuptItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;

	SetReplicates(true);
}

void ABuptItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLipOpened=!bLipOpened;
	OnRep_LidOpened();
}

void ABuptItemChest::OnActorLoaded_Implementation()
{
	IBuptGamePlayInterface::OnActorLoaded_Implementation();
	OnRep_LidOpened();
}

void ABuptItemChest::OnRep_LidOpened()
{
	float CurrentPitch=bLipOpened?TargetPitch:0.0f;

	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

void ABuptItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuptItemChest,bLipOpened);
}




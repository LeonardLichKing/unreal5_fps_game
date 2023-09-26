// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptTargetDummy.h"

// Sets default values
ABuptTargetDummy::ABuptTargetDummy()
{
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent=MeshComp;

	AttributeComp=CreateDefaultSubobject<UBuptAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this,&ABuptTargetDummy::OnHealthChanged);
}

void ABuptTargetDummy::OnHealthChanged(AActor* InstigatorActor, UBuptAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta<0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
	}
}




// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "BuptTargetDummy.generated.h"

class UBuptAttributeComponent;

UCLASS()
class FPSPROJECT_API ABuptTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuptTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UBuptAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,UBuptAttributeComponent* OwningComp,float NewHealth,float Delta);
};

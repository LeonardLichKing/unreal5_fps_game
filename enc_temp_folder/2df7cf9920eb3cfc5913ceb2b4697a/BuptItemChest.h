// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuptGamePlayInterface.h"
#include "BuptItemChest.generated.h"

UCLASS()
class FPSPROJECT_API ABuptItemChest : public AActor,public IBuptGamePlayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnyWhere)
	float TargetPitch;
	void Interact_Implementation(APawn* InstigatorPawn);
public:	
	// Sets default values for this actor's properties
	ABuptItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

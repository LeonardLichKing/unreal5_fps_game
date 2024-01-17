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

	void OnActorLoaded_Implementation();
public:	
	// Sets default values for this actor's properties
	ABuptItemChest();

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened",BlueprintReadOnly,SaveGame)
	bool bLipOpened;

	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;


};

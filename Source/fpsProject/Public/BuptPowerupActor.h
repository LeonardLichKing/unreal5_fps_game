// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptGamePlayInterface.h"
#include "GameFramework/Actor.h"
#include "BuptPowerupActor.generated.h"
//此类是所有交互物品的基类，后续要实现的类的都继承于此类，因此此类需要定义所有可交互物品的共同操作

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class FPSPROJECT_API ABuptPowerupActor : public AActor,public IBuptGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuptPowerupActor();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleAnywhere,Category="Components")
	USphereComponent* SphereComp;

	// UPROPERTY(VisibleAnywhere,Category="Components")
	// UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere,Category="Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bIsActivate);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptMagicProjectile.h"
#include "BuptDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API ABuptDashProjectile : public ABuptMagicProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DetonateDelay;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, FHitResult Hit);

	void Explode();

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:
	ABuptDashProjectile();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptProjectileBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "BuptMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UActorChannel;
class UCameraShakeBase;

UCLASS()
class ABuptMagicProjectile : public ABuptProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuptMagicProjectile();

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditDefaultsOnly,Category="Effects")
	float Damage;

	UPROPERTY(EditDefaultsOnly,Category="Effects")
	FGameplayTag ParryTag;
};

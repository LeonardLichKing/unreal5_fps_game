// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuptAction.h"
#include "BuptAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBuptAction_ProjectileAttack : public UBuptAction
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack");
	TSubclassOf<AActor>ProjectileClass;

	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere,Category="Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	UBuptAction_ProjectileAttack();
};

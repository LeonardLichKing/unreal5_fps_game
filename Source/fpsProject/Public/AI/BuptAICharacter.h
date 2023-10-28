// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BuptAICharacter.generated.h"

class UPawnSensingComponent;
class UBuptAttributeComponent;

UCLASS()
class FPSPROJECT_API ABuptAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABuptAICharacter();

protected:
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,UBuptAttributeComponent* OwningComp,float NewHealth,float Delta);
	
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UBuptAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};

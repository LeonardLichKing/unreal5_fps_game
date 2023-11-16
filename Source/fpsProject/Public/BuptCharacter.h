// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BuptCharacter.generated.h"

class UBuptActionrComponent;
class UCameraComponent;
class USpringArmComponent;
class UBuptInteractionComponent;
class UAnimMontage;
class UBuptAttributeComponent;

UCLASS()
class FPSPROJECT_API ABuptCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,UBuptAttributeComponent* OwningComp,float NewHealth,float Delta);

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	TSubclassOf<UCameraShakeBase> CameraShakeComp;

public:
	// Sets default values for this character's properties
	ABuptCharacter();

	UFUNCTION(Exec)
	void HealSelf(float Amount=100);
	
protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UBuptInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBuptAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UBuptActionrComponent* ActionComp;

	void MoveForward(float value);

	void MoveRight(float value);

	void SprintStart();

	void SprintStop();

	void PrimaryAttack();

	void BlackHole();

	void Dash();

	void Jump();

	void PrimaryInteract();

	virtual FVector GetPawnViewLocation() const override;
public:	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

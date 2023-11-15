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

	UPROPERTY(EditAnywhere, Category = "Attack");//使得可以在ue编辑器中进行设置和编辑
	TSubclassOf<AActor>ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack");//使得可以在ue编辑器中进行设置和编辑
	TSubclassOf<AActor>ProjectileClassBlackHole;

	UPROPERTY(EditAnywhere, Category = "Attack");//使得可以在ue编辑器中进行设置和编辑
	TSubclassOf<AActor>ProjectileClassDash;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	TSubclassOf<UCameraShakeBase> CameraShakeComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_BlackHole;

	FTimerHandle TimerHandle_Dash;
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	void SprintStart();

	void SprintStop();

	void SpawnProjectile(TSubclassOf<AActor>ClassToSpawn);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void BlackHole();

	void BlackHole_TimeElapsed();

	void Dash();

	void Dash_TimeElapsed();

	void Jump();

	void PrimaryInteract();

	virtual FVector GetPawnViewLocation() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

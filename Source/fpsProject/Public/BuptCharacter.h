// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BuptCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;



UCLASS()
class FPSPROJECT_API ABuptCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere);//ʹ�ÿ�����ue�༭���н������úͱ༭
	TSubclassOf<AActor>ProjectileClass;

public:
	// Sets default values for this character's properties
	ABuptCharacter();

protected:
	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere);
	UCameraComponent* CameraComp;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();
	void Jump();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

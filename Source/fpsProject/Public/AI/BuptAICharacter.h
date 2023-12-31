// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BuptAICharacter.generated.h"

class UBuptActionrComponent;
class UBuptWorldUserWidget;
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

	UBuptWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,UBuptAttributeComponent* OwningComp,float NewHealth,float Delta);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;
	
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UBuptAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UBuptActionrComponent* ActionComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

};

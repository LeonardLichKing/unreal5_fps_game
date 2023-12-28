// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuptAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UBuptAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, UBuptAttributeComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UBuptAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFUNCTION(BlueprintCallable,Category="Attributes")
	static UBuptAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable,Category="Atttributes",meta=(DispalayName="IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	UBuptAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float HealthMax;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Attributes")
	float RageMax;

public:

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable,Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor,float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRage(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable,Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable,Category = "Attributes")
	bool IsInjured() const;

	UFUNCTION(BlueprintCallable,Category = "Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable,Category="Attributes")
	float GetRage() const;

	UFUNCTION(BlueprintCallable,Category = "Attributes")
	float GetMaxHealth() const;
};

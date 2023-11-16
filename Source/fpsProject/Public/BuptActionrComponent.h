// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuptActionrComponent.generated.h"


class UBuptAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UBuptActionrComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFUNCTION(BlueprintCallable,Category="Actions")
	void AddAction(TSubclassOf<UBuptAction> ActionClass);

	UFUNCTION(BlueprintCallable,Category="Actions")
	bool StartActionByName(AActor* Instigator,FName ActionName);

	UFUNCTION(BlueprintCallable,Category="Actions")
	bool StopActionByName(AActor* Instigator,FName ActionName);
	
	UBuptActionrComponent();

protected:
	// Called when the game starts
	UPROPERTY(EditAnywhere,Category="Actions")
	TArray<TSubclassOf<UBuptAction>> DefaultActions;
	
	UPROPERTY()
	TArray<UBuptAction*> Actions;
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

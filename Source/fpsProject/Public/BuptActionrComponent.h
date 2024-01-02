// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "BuptActionrComponent.generated.h"


class UBuptAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UBuptActionrComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	UFUNCTION(BlueprintCallable,Category="Actions")
	void AddAction(AActor* Instigator,TSubclassOf<UBuptAction> ActionClass);

	UFUNCTION(BlueprintCallable,Category="Actions")
	void RemoveAction(UBuptAction* ActionToRemove);

	/* Returns first occurrence of action matching the class provided */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	UBuptAction* GetAction(TSubclassOf<UBuptAction> ActionClass) const;
	
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

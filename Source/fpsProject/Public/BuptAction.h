// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "BuptAction.generated.h"

class UBuptActionrComponent;
class UWorld;
/**
 * 
 */
UCLASS(Blueprintable)
class FPSPROJECT_API UBuptAction : public UObject
{
	GENERATED_BODY()
protected:

	UFUNCTION(BlueprintCallable,Category="Action")
	UBuptActionrComponent* GetOwningComponent() const;
	
	//当action发生时加入容器，结束后移除
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantTags;

	//只有当action标签不在容器中时action才能执行
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;
	
public:
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StopAction(AActor* Instigator);
	
	//Action nickname to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly,Category="Action")
	FName ActionName;

	UWorld* GetWorld() const override;
};

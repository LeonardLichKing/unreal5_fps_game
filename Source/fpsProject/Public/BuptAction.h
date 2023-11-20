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
	
	//��action����ʱ�����������������Ƴ�
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantTags;

	//ֻ�е�action��ǩ����������ʱaction����ִ��
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

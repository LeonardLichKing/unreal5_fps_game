// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "BuptAction.generated.h"

class UBuptActionrComponent;
class UWorld;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class FPSPROJECT_API UBuptAction : public UObject
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	UTexture2D* Icon;

	UPROPERTY(Replicated)
	UBuptActionrComponent* ActionComp;
	
	UFUNCTION(BlueprintCallable,Category="Action")
	UBuptActionrComponent* GetOwningComponent() const;
	
	//��action����ʱ�����������������Ƴ�
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantTags;

	//ֻ�е�action��ǩ����������ʱaction����ִ��
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	// bool bIsRunning;
	
	float TimeStarted;
	
	UFUNCTION()
	void OnRep_RepData();
	
public:

	void Initialize(UBuptActionrComponent* NewActionComp);
	
	UPROPERTY(EditDefaultsOnly,Category="Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintCallable,Category="Action")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Action")
	void StopAction(AActor* Instigator);
	
	//Action nickname to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly,Category="Action")
	FName ActionName;

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};

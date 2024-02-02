// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAnimInstance.h"
#include "BuptActionrComponent.h"
#include "GameplayTagContainer.h"

void UBuptAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	AActor* OwningActor=GetOwningActor();
	if(OwningActor)
	{
		ActionComp=Cast<UBuptActionrComponent>(OwningActor->GetComponentByClass(UBuptActionrComponent::StaticClass()));
	}
}

void UBuptAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag=FGameplayTag::RequestGameplayTag("Status.Stunned");
	if(ActionComp)
	{
		bIsStunned=ActionComp->ActiveGameplayTags.HasTag(StunnedTag);

		UE_LOG(LogTemp, Warning, TEXT("bIsStunned is: %s"), bIsStunned ? TEXT("true") : TEXT("false"));
		// 打印出 ActiveGameplayTags 中的所有标签，以确保 "Status.Stunned" 存在
		for (const FGameplayTag& Tag : ActionComp->ActiveGameplayTags)
		{
			UE_LOG(LogTemp, Warning, TEXT("Active Gameplay Tag: %s"), *Tag.ToString());
		}
	}
}

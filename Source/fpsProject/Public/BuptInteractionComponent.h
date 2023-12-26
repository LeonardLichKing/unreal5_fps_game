// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuptInteractionComponent.generated.h"


class UBuptWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UBuptInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();

protected:

	void FindBestInteractable();

	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UBuptWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UBuptWorldUserWidget> DefaultWidgetInstance;
	
public:	
	// Sets default values for this component's properties
	UBuptInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};

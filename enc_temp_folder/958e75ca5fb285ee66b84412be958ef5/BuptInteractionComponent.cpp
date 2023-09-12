// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptInteractionComponent.h"
#include "BuptGamePlayInterface.h"

// Sets default values for this component's properties
UBuptInteractionComponent::UBuptInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBuptInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBuptInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuptInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	//FVector Start;我们可以直接使用eyelocation来代替射线的起点位置，可以理解为从眼睛发射一条有固定距离的线
	//当这条线的末端与treasure chest发生hit事件时，treasure chest就触发打开的事件

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);


	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		if (HitActor->Implements<UBuptGamePlayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);

			IBuptGamePlayInterface::Execute_Interact(HitActor, MyPawn);

		}
	}
}

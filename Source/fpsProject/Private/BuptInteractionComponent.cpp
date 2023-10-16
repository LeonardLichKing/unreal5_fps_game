// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptInteractionComponent.h"
#include "BuptGamePlayInterface.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"


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

	// FVector EyeLocation;
	// FRotator EyeRotation;
	// MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	//
	// FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
	FVector CameraLocation;
	FRotator CameraRotation;
	UCameraComponent* CameraComp=Cast<UCameraComponent>(MyOwner->GetComponentByClass(UCameraComponent::StaticClass()));
	CameraLocation=CameraComp->GetComponentLocation();
	CameraRotation=CameraComp->GetComponentRotation();
	FVector End=CameraLocation+(CameraRotation.Vector()*300);
	

	//FHitResult Hit;
	//bool bBlockingHit=GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	//bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, CameraLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
			
			if (HitActor->Implements<UBuptGamePlayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);

				IBuptGamePlayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}

	
	
	// DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	DrawDebugLine(GetWorld(), CameraLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}

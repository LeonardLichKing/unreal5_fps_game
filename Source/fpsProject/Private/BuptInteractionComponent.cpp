// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptInteractionComponent.h"
#include "BuptGamePlayInterface.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"),false,TEXT("Enable Debug Lines for Interact Component."),ECVF_Cheat);

// Sets default values for this component's properties
UBuptInteractionComponent::UBuptInteractionComponent()
{
	
}


void UBuptInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw=CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
	// FVector CameraLocation;
	// FRotator CameraRotation;
	// UCameraComponent* CameraComp=Cast<UCameraComponent>(MyOwner->GetComponentByClass(UCameraComponent::StaticClass()));
	// CameraLocation=CameraComp->GetComponentLocation();
	// CameraRotation=CameraComp->GetComponentRotation();
	// FVector End=CameraLocation+(CameraRotation.Vector()*500);
	

	//FHitResult Hit;
	//bool bBlockingHit=GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	//bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if(bDebugDraw)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
			}
			
			if (HitActor->Implements<UBuptGamePlayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);

				IBuptGamePlayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}

	
	
	// DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}

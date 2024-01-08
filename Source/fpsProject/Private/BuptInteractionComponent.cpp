// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptInteractionComponent.h"
#include "BuptGamePlayInterface.h"
#include "BuptWorldUserWidget.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"),false,TEXT("Enable Debug Lines for Interact Component."),ECVF_Cheat);

// Sets default values for this component's properties
UBuptInteractionComponent::UBuptInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick=true;

	TraceDistance=500.0f;
	TraceRadius=30.0f;
	CollisionChannel=ECC_WorldDynamic;
}

void UBuptInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBuptInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn=Cast<APawn>(GetOwner());

	//如果是后台服务器同时作为前台作为客户端（聆听），那么是true，如果是纯后台服务器（独立）那么是false。
	if(MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UBuptInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw=CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor=nullptr;
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if(bDebugDraw)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
			}
			
			if (HitActor->Implements<UBuptGamePlayInterface>())
			{
				FocusedActor=HitActor;
				break;
			}
		}
	}

	if(FocusedActor)
	{
		if(DefaultWidgetInstance==nullptr&&ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UBuptWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor=FocusedActor;

			if(!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
		
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}

void UBuptInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UBuptInteractionComponent::ServerInteract_Implementation(AActor* InActor)
{
	if(InActor==nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,"No Focus Actor to interact");
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());

	IBuptGamePlayInterface::Execute_Interact(InActor, MyPawn);
}

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

	//FVector Start;���ǿ���ֱ��ʹ��eyelocation���������ߵ����λ�ã��������Ϊ���۾�����һ���й̶��������
	//�������ߵ�ĩ����treasure chest����hit�¼�ʱ��treasure chest�ʹ����򿪵��¼�

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

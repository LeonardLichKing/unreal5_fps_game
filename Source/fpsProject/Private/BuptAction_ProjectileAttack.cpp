// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GamePlayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBuptAction_ProjectileAttack::UBuptAction_ProjectileAttack()
{
	AttackAnimDelay=0.2f;

	HandSocketName="Muzzle_01";
}

void UBuptAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		//����line trace���߼����ײ������
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FHitResult Hit;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//����player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		
		//�����������ת�������������ת�󶨵ģ����Կ���ֱ��ͨ����������λ�úͷ��������׼�ĵ�λ���볯��
		FVector CameraLocation = InstigatorCharacter->GetPawnViewLocation();

		//���ߵĳ����ɽ�ɫ�Ĺ�����Χ����
		int AttackRange = 5000;
		FVector End = CameraLocation + (InstigatorCharacter->GetControlRotation().Vector() * AttackRange);

		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, CameraLocation, End, FQuat::Identity,ObjectQueryParams,Shape,Params);
		//DrawDebugLine(GetWorld(), CameraLocation, End, FColor::Red, true, 2.0f, 0, 2.0f);


		FTransform SpawnTM;

		FRotator Bias;

		if (bBlockingHit)
		{
			//������߱��赲��˵���������ϰ����������ײ
			FVector HitPoint = Hit.ImpactPoint;
			Bias = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitPoint);
			//DrawDebugSphere(GetWorld(), HitPoint, 20.0f, 16, FColor::Red, true);
		}
		else
		{
			Bias = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
		}


		SpawnTM = FTransform(Bias, HandLocation);//para1��ʾ��ÿ���������ķ���para2��ʾ����λ��

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//�����Ƿ��������Ƿ����ײ������
		SpawnParams.Instigator = InstigatorCharacter;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	StopAction(InstigatorCharacter);
}

void UBuptAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character=Cast<ACharacter>(Instigator);
	if(Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect,Character->GetMesh(),HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"AttackDelay_Elapsed",Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate,AttackAnimDelay,false);
	}
	
}


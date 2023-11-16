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

		//设置line trace射线检测碰撞的类型
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FHitResult Hit;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//无视player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		
		//由于相机的旋转是与控制器的旋转绑定的，所以可以直接通过获得相机的位置和方向来获得准心的位置与朝向
		FVector CameraLocation = InstigatorCharacter->GetPawnViewLocation();

		//射线的长度由角色的攻击范围决定
		int AttackRange = 5000;
		FVector End = CameraLocation + (InstigatorCharacter->GetControlRotation().Vector() * AttackRange);

		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, CameraLocation, End, FQuat::Identity,ObjectQueryParams,Shape,Params);
		//DrawDebugLine(GetWorld(), CameraLocation, End, FColor::Red, true, 2.0f, 0, 2.0f);


		FTransform SpawnTM;

		FRotator Bias;

		if (bBlockingHit)
		{
			//如果射线被阻挡，说明遇到了障碍物，发生了碰撞
			FVector HitPoint = Hit.ImpactPoint;
			Bias = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitPoint);
			//DrawDebugSphere(GetWorld(), HitPoint, 20.0f, 16, FColor::Red, true);
		}
		else
		{
			Bias = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
		}


		SpawnTM = FTransform(Bias, HandLocation);//para1表示获得控制器面向的方向，para2表示发射位置

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//不考虑发射物体是否会碰撞的问题
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


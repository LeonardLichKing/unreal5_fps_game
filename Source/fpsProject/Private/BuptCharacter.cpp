// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BuptInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


// Sets default values
ABuptCharacter::ABuptCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;//摄像机的转向跟随控制器
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UBuptInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;//让charactor自动转向控制移动的方向

	bUseControllerRotationYaw = false;//仅仅旋转控制器视角的时候角色不跟着旋转，而是保持朝向
}

// Called when the game starts or when spawned
void ABuptCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuptCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	//以上操作获得了controller的方向，并且限制在水平方向旋转


	AddMovementInput(ControlRot.Vector(), value);//将移动方向调整为controller的方向，而不是角色自身的方向
}


void ABuptCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	//同样将右转限定在水平方向
	//x = forword (red)
	//y = right (green)
	//z = up (blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}


void ABuptCharacter::PrimaryAttack()
{
	
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ABuptCharacter::PrimaryAttack_TimeElapsed, 0.2f);

}

void ABuptCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	//设置line trace射线检测碰撞的类型
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FHitResult Hit;

	//由于相机的旋转是与控制器的旋转绑定的，所以可以直接通过获得相机的位置和方向来获得准心的位置与朝向
	FVector CameraLocation = CameraComp->GetComponentLocation();
	FRotator CameraRotator = CameraComp->GetComponentRotation();

	//射线的长度由角色的攻击范围决定
	int AttackRange=5000;
	FVector End = CameraLocation + (CameraRotator.Vector() * AttackRange);

	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, End, ObjectQueryParams);
	//DrawDebugLine(GetWorld(), CameraLocation, End, FColor::Red, true, 2.0f, 0, 2.0f);


	FTransform SpawnTM;

	FRotator Bias;

	if (bBlockingHit)
	{
		//如果射线被阻挡，说明遇到了障碍物，此时Hit记录的是block的信息
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
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ABuptCharacter::Jump()
{
	if (CanJump())
	{
		LaunchCharacter(FVector(0.0f, 0.0f, 500.0f), false, false);
	}
}

void ABuptCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

// Called every frame
void ABuptCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABuptCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ABuptCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABuptCharacter::MoveRight);


	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ABuptCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABuptCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ABuptCharacter::PrimaryInteract);
}


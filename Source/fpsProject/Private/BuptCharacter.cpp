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
	SpringArmComp->bUsePawnControlRotation = true;//�������ת����������
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UBuptInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;//��charactor�Զ�ת������ƶ��ķ���

	bUseControllerRotationYaw = false;//������ת�������ӽǵ�ʱ���ɫ��������ת�����Ǳ��ֳ���
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
	//���ϲ��������controller�ķ��򣬲���������ˮƽ������ת


	AddMovementInput(ControlRot.Vector(), value);//���ƶ��������Ϊcontroller�ķ��򣬶����ǽ�ɫ����ķ���
}


void ABuptCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	//ͬ������ת�޶���ˮƽ����
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

	//����line trace���߼����ײ������
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FHitResult Hit;

	//�����������ת�������������ת�󶨵ģ����Կ���ֱ��ͨ����������λ�úͷ��������׼�ĵ�λ���볯��
	FVector CameraLocation = CameraComp->GetComponentLocation();
	FRotator CameraRotator = CameraComp->GetComponentRotation();

	//���ߵĳ����ɽ�ɫ�Ĺ�����Χ����
	int AttackRange=5000;
	FVector End = CameraLocation + (CameraRotator.Vector() * AttackRange);

	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, End, ObjectQueryParams);
	//DrawDebugLine(GetWorld(), CameraLocation, End, FColor::Red, true, 2.0f, 0, 2.0f);


	FTransform SpawnTM;

	FRotator Bias;

	if (bBlockingHit)
	{
		//������߱��赲��˵���������ϰ����ʱHit��¼����block����Ϣ
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


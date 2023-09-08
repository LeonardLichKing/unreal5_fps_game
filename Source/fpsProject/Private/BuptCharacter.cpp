// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}


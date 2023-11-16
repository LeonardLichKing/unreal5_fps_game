// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptCharacter.h"

#include "BuptActionrComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BuptInteractionComponent.h"
#include "BuptAttributeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/GameSession.h"


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

	AttributeComp=CreateDefaultSubobject<UBuptAttributeComponent>("AttributeComp");

	ActionComp=CreateDefaultSubobject<UBuptActionrComponent>("ActionComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;//让charactor自动转向控制移动的方向
	

	bUseControllerRotationYaw = false;//仅仅旋转控制器视角的时候角色不跟着旋转，而是保持朝向
}

void ABuptCharacter::HealSelf(float Amount /*+100*/)
{
	AttributeComp->ApplyHealthChange(this,Amount);
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

void ABuptCharacter::SprintStart()
{
	ActionComp->StartActionByName(this,"Sprint");
}

void ABuptCharacter::SprintStop()
{
	ActionComp->StopActionByName(this,"Sprint");
}

void ABuptCharacter::PrimaryAttack()
{	
	ActionComp->StartActionByName(this,"PrimaryAttack");
}

void ABuptCharacter::BlackHole()
{
	ActionComp->StartActionByName(this,"BlackHole");
}

void ABuptCharacter::Dash()
{
	ActionComp->StartActionByName(this,"Dash");
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

FVector ABuptCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();	
}

void ABuptCharacter::OnHealthChanged(AActor* InstigatorActor, UBuptAttributeComponent* OwningComp, float NewHealth,
                                     float Delta)
{
	if(Delta<0&&NewHealth>0)
	{
		APlayerController* PCController=GetWorld()->GetFirstPlayerController();//TODO:should take care of multi-player
		PCController->ClientStartCameraShake(CameraShakeComp);
	}
	if(NewHealth<=0&&Delta<=0)
	{
		APlayerController* PC=Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}
void ABuptCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this,&ABuptCharacter::OnHealthChanged);
}

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
	PlayerInputComponent->BindAction("BlackHole", IE_Pressed, this, &ABuptCharacter::BlackHole);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABuptCharacter::Dash);

	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&ABuptCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&ABuptCharacter::SprintStop);
}


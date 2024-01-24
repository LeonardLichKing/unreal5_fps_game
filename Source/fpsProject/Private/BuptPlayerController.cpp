// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptPlayerController.h"

#include "Blueprint/UserWidget.h"

void ABuptPlayerController::TogglePauseMenu()
{
	if(PauseMenuInstance&&PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance=nullptr;
		bShowMouseCursor=false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	PauseMenuInstance=CreateWidget<UUserWidget>(this,PauseMenuClass);
	if(PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor=true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ABuptPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("PauseMenu",IE_Pressed,this,&ABuptPlayerController::TogglePauseMenu);
}

void ABuptPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void ABuptPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	BlueprintBeginPlayingState();
}

void ABuptPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateReceived.Broadcast(PlayerState);
}

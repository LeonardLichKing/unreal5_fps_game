// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptPlayerController.h"

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

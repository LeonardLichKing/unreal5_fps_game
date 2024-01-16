// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptPlayerController.h"

void ABuptPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

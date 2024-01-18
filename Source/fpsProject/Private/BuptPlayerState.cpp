// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptPlayerState.h"

#include "BuptSaveGame.h"
#include "Net/UnrealNetwork.h"

void ABuptPlayerState::AddCredits(int32 Delta)
{
	if(!ensure(Delta>0.0f))
	{
		return;
	}

	Credits+=Delta;

	OnCreditsChanged.Broadcast(this,Credits,Delta);
}

bool ABuptPlayerState::RemoveCredits(int32 Delta)
{
	if(!ensure(Delta>0.0f))
	{
		return false;
	}

	if(Credits<Delta)
	{
		return false;
	}

	Credits-=Delta;

	OnCreditsChanged.Broadcast(this,Credits,-Delta);

	return true;
}

void ABuptPlayerState::LoadPlayerState_Implementation(UBuptSaveGame* SaveObject)
{
	if(SaveObject)
	{
		// Credits=SaveObject->Credits;
		//这样做的目的是触发OnCreditsChanged事件，以便于UI更新
		AddCredits(SaveObject->Credits);
	}
}

void ABuptPlayerState::SavePlayerState_Implementation(UBuptSaveGame* SaveObject)
{
	if(SaveObject)
	{
		SaveObject->Credits=Credits;
	}
}

int32 ABuptPlayerState::GetCredits() const
{
	return Credits;
}

void ABuptPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ABuptPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuptPlayerState,Credits);
}

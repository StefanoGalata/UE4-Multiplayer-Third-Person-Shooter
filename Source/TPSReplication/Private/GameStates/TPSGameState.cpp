// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/TPSGameState.h"
#include "Net/UnrealNetwork.h"

void ATPSGameState::SetWaveState(EWaveState NewWaveState)
{
	if (HasAuthority())
	{
		EWaveState OldState = WaveState;
		WaveState = NewWaveState;

		// Called on Server
		OnRep_WaveState(OldState);
	}
}

void ATPSGameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

void ATPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSGameState, WaveState);
}
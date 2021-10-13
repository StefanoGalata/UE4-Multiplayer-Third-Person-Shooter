// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/TPSGameState.h"
#include "Net/UnrealNetwork.h"

void ATPSGameState::OnRep_WaveState(EWaveState OldState)
{

}

void ATPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSGameState, WaveState);
}
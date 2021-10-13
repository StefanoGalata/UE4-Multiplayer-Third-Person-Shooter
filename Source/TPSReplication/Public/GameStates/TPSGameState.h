// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TPSGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,
	WaveInProgress,

	// No longer spawning new bots, waiting for player to kill all remaining bots
	WaitingToComplete,

	WaveComplete,

	GameOver
};

/**
 * 
 */
UCLASS()
class TPSREPLICATION_API ATPSGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	void SetWaveState(EWaveState NewWaveState);

protected:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState WaveState;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);
};

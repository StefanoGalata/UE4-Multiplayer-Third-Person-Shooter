// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameMode.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

/**
 * The game mode only exists on the Server
 */
UCLASS()
class TPSREPLICATION_API ATPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void StartPlay() override;


	void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;

protected:

	ATPSGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves = 3.f;

	int32 NumberOfBotsToSpawn = 0;

	int32 WaveCount = 0;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();
	
	void StartWave();

	void EndWave();

	// Set timer for next wave
	void PrepareForNextWave();

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewWaveState);

	void RestartDeadPlayers();


	FTimerHandle TimerHandle_BotSpawner;
	FTimerHandle TimerHandle_NextWaveStart;

private:
	bool bIsAnyBotAlive = false;

};

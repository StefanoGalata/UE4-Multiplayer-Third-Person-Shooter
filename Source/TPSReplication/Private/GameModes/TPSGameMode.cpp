// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TPSGameMode.h"
#include "TimerManager.h"
#include "GameStates/TPSGameState.h"
#include "Components/HealthComponent.h"

ATPSGameMode::ATPSGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;

	GameStateClass = ATPSGameState::StaticClass();
}

void ATPSGameMode::StartPlay()
{
	Super::StartPlay();
	PrepareForNextWave();
}


void ATPSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckWaveState();
	CheckAnyPlayerAlive();
}

void ATPSGameMode::StartWave()
{
	++WaveCount;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ATPSGameMode::SpawnBotTimerElapsed, 1.f, true);
}

void ATPSGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	PrepareForNextWave();
}

void ATPSGameMode::PrepareForNextWave()
{
	
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ATPSGameMode::StartWave, TimeBetweenWaves, false);
}

void ATPSGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart); 
	
	if (NumberOfBotsToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		UHealthComponent* HealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetCurrentHealth()>0.f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		PrepareForNextWave();
	}
}

void ATPSGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();

		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			UHealthComponent* HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetCurrentHealth() > 0.f)
			{
				return;
			}
		}
	}

	// No player alive
	GameOver();
}

void ATPSGameMode::GameOver()
{
	EndWave();

	// @TODO: FInish the match

	UE_LOG(LogTemp, Log, TEXT("Game Over, Players Died"));
}

void ATPSGameMode::SetWaveState(EWaveState NewWaveState)
{
	ATPSGameState* GS = GetGameState<ATPSGameState>();
	if (ensureAlways(GS))
	{
		GS->WaveState = NewWaveState;
	}
}

void ATPSGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	--NumberOfBotsToSpawn;
	if (NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

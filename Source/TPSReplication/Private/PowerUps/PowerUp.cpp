// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUps/PowerUp.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PowerUpInterval = 0.f;
	TotalNumberOfTicks = 0;
}

void APowerUp::ActivatePowerUp()
{
	if (PowerUpInterval > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerUpTick, this, &APowerUp::OnTickPowerup, PowerUpInterval, true, 0.f);

	}
	else
	{
		OnTickPowerup();
	}
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
}

void APowerUp::OnTickPowerup()
{
	++TicksProcessed;

	OnPowerupTicked();

	if (TicksProcessed >= TotalNumberOfTicks)
	{
		OnExpired();

		// Delete timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTick);
	}
}


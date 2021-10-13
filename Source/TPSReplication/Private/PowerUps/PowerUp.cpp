// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUps/PowerUp.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PowerUpInterval = 0.f;
	TotalNumberOfTicks = 0;
	bReplicates = true;
}

void APowerUp::ActivatePowerUp(AActor* ActiveFor)
{

	OnActivated(ActiveFor);

	bIsPowerUpActive = true;
	OnRep_PowerUpActive();

	if (PowerUpInterval > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerUpTick, this, &APowerUp::OnTickPowerup, PowerUpInterval, true);

	}
	else
	{
		OnTickPowerup();
	}
}

void APowerUp::OnRep_PowerUpActive()
{
	OnPowerUpStateChanged(bIsPowerUpActive);
}

void APowerUp::OnTickPowerup()
{
	++TicksProcessed;

	OnPowerupTicked();

	if (TicksProcessed >= TotalNumberOfTicks)
	{

		bIsPowerUpActive = false;
		OnRep_PowerUpActive();

		OnExpired();

		// Delete timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTick);
	}
}

void APowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerUp, bIsPowerUpActive)
}



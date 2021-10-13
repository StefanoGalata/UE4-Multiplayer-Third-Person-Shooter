// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UCLASS()
class TPSREPLICATION_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnActivated(AActor* ActiveFor);

	void ActivatePowerUp(AActor* ActiveFor);

protected:

	// Time between power up ticks
	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	float PowerUpInterval;

	// Total times we apply power up effects
	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	int32 TotalNumberOfTicks;

	// Number of ticks applied
	int32 TicksProcessed = 0;

	UFUNCTION()
	void OnRep_PowerUpActive();

	UPROPERTY(ReplicatedUsing = OnRep_PowerUpActive)
	bool bIsPowerUpActive = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnPowerUpStateChanged(bool bNewIsActive);

private:
	
	FTimerHandle TimerHandle_PowerUpTick;

	UFUNCTION()
	void OnTickPowerup();

};

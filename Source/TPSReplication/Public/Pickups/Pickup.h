// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class USphereComponent;
class APowerUp;

UCLASS()
class TPSREPLICATION_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UDecalComponent* DecalComp;  

	UPROPERTY(EditInstanceOnly, Category = "Pickup")
	TSubclassOf<APowerUp> PowerUpClass;

	UPROPERTY(EditInstanceOnly, Category = "Pickup")
	float CooldownDuration = 10.f;

private:

	APowerUp* PowerUpInstance;

	FTimerHandle TimerHandle_RespawnTime;

	void Respawn();


};

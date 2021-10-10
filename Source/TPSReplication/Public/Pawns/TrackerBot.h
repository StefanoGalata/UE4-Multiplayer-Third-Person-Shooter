// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackerBot.generated.h"

class UHealthComponent;
class USphereComponent;

UCLASS()
class TPSREPLICATION_API ATrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrackerBot();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage = 40.f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelfDamageInterval = .25f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce = 1000.f;

	// Distance to check for nearby bots
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float NearbyBotSearchRadius = 600.f;

	// Max power level when it is near other bots
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	int32 MaxPowerLevel = 4;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange = false;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundBase* SelfDestructSFX;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundBase* ExplodeSFX;

	UFUNCTION()
	void HandleTakeDamage(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();

	// Dynamic material to pulse on damage
	UMaterialInstanceDynamic* MatInst;

	FName MaterialParameterDamagePulseName = "LastTimeDamageTaken";

	void SelfDestruct();

	void OnCheckNearbyBots();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


private:

	int32 PowerLevel = 0; 

	FVector NextPathPoint;

	bool bExploded = false;

	bool bStartedSelfDestruction = false;

	FTimerHandle TimerHandle_SelfDamage;

	void DamageSelf();
};

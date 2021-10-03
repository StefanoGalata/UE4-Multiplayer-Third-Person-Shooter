// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UCameraShakeBase;

UCLASS()
class TPSREPLICATION_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	virtual void Fire();

	void StartFire();

	void StopFire();

	

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float BaseDamage = 20.f;

	/**
	 * Bullets per minute fired
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RateOfFire;

	// Derived from rate of fire
	float TimeBetweenShots;

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFiredTime;
	
private:
	void PlayFireEffects(FVector TraceEnd);
};

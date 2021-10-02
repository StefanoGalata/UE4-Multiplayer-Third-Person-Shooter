// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TPSREPLICATION_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Weapon")
	TSubclassOf<AActor> ProjectileClass;
	
};

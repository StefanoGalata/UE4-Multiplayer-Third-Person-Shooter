// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeapon.h"

void AProjectileWeapon::Fire()
{
	AActor* MyOwner = GetOwner();

	if (MyOwner && ProjectileClass)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		
		FVector MuzzleLocation = Mesh->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = Mesh->GetSocketRotation(MuzzleSocketName);

		FActorSpawnParameters SpawnParams;

		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);
	}
}
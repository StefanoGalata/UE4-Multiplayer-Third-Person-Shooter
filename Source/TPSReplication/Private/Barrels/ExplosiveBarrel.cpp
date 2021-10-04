// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrels/ExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = Mesh;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComponent->SetupAttachment(Mesh);
	RadialForceComponent->Radius = 250.f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;


	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	ExplosionImpulse = 400.f;
}

void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AExplosiveBarrel::OnHealthChanged);
	}
}

void AExplosiveBarrel::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bExploded)
	{
		return;
	}

	if (Health <= 0)
	{
		bExploded = true;

		// Boost the barrel upwards
		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		Mesh->AddImpulse(BoostIntensity, NAME_None, true);

		// Play FX and change self material
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		Mesh->SetMaterial(0, ExplodedMaterial);

		RadialForceComponent->FireImpulse();

		// @TODO: Apply radial damage
	}
}

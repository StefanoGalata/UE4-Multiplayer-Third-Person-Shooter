// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	MuzzleSocketName = "MuzzleSocket";

	TracerTargetName = "Target";

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Fire()
{

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEnd = EyeLocation + (ShotDirection * 1000.f);


		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		//Particle "target" parameter
		FVector TracerEndPoint = TraceEnd;

		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 20.f, ShotDirection, HitResult, MyOwner->GetInstigatorController(), this, DamageType);

			if (ImpactEffect != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
			}
			TracerEndPoint = HitResult.ImpactPoint;
		}

		//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);

		if (MuzzleEffect != nullptr)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, Mesh, MuzzleSocketName);
		}


		if (TracerEffect != nullptr)
		{
			FVector MuzzleLocation = Mesh->GetSocketLocation(MuzzleSocketName);

			UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
			
			if (TracerComponent != nullptr)
			{
				TracerComponent->SetVectorParameter(TracerTargetName, TracerEndPoint);
			}
		}
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


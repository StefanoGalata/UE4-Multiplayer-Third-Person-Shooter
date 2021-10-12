// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PowerUps/PowerUp.h"

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComp->SetSphereRadius(75.f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	DecalComp->SetupAttachment(SphereComp);
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	Respawn();
}

void APickup::Respawn()
{
	if (PowerUpClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No power up class in %s. Upadte your BP"), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerUpInstance = GetWorld()->SpawnActor<APowerUp>(PowerUpClass, GetTransform(), SpawnParams);
}

void APickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// Grant a power up
	if (PowerUpInstance)
	{
		PowerUpInstance->ActivatePowerUp();
		PowerUpInstance = nullptr;

		// set timer to respawn powerup
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTime, this, &APickup::Respawn, CooldownDuration);
	}
}



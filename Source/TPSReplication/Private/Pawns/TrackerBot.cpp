// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/TrackerBot.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/HealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"



// Sets default values
ATrackerBot::ATrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCanEverAffectNavigation(false);
	Mesh->SetSimulatePhysics(true);
	RootComponent = Mesh;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Spere Component"));
	SphereComp->SetSphereRadius(200.f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void ATrackerBot::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &ATrackerBot::HandleTakeDamage);
	}

	// Find initial move-to point
	NextPathPoint = GetNextPathPoint();
}

void ATrackerBot::HandleTakeDamage(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if(MatInst == nullptr)
	{
		MatInst = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Mesh->GetMaterial(0));
	}

	if (MatInst)
	{
		MatInst->SetScalarParameterValue(MaterialParameterDamagePulseName, GetWorld()->GetTimeSeconds());
	}

	if (Health <= 0.f)
	{
		SelfDestruct();
	}
}

FVector ATrackerBot::GetNextPathPoint()
{
	// @TODO Find the actual target
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

	if (NavPath->PathPoints.Num() > 1)
	{
		// return the next point in the path
		return NavPath->PathPoints[1];
	}

	// Failed to find  path
	return GetActorLocation();
}

void ATrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}

	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 5.f, 0, 3.f);

	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSFX, GetActorLocation());

	Destroy();
}


void ATrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (bStartedSelfDestruction)
	{
		return;
	}
	APawn* OverlappedPawn = Cast<APawn>(OtherActor);
	if (OverlappedPawn->IsPlayerControlled())
	{
		// Overlapped a player

		// Start self destruction sequence
		GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this,&ATrackerBot::DamageSelf, SelfDamageInterval, true);

		bStartedSelfDestruction = true;

		UGameplayStatics::SpawnSoundAttached(SelfDestructSFX, RootComponent);
	}
}

void ATrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20.f, GetInstigatorController(), this, nullptr);
}

// Called every frame
void ATrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (GetActorLocation()-NextPathPoint).Size();

	if (DistanceToTarget <= RequiredDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{

		// Keep moving to target

		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();

		Mesh->AddForce(ForceDirection * MovementForce, NAME_None, bUseVelocityChange);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/TrackerBot.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/HealthComponent.h"
#include "Engine.h"

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
	// @TODO pulse material on hit

	// @TODO Explode on death

	if(MatInst == nullptr)
	{
		MatInst = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Mesh->GetMaterial(0));
	}

	if (MatInst)
	{
		MatInst->SetScalarParameterValue(MaterialParameterDamagePulseName, GetWorld()->GetTimeSeconds());
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

// Called every frame
void ATrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (GetActorLocation()-NextPathPoint).Size();

	if (DistanceToTarget <= RequiredDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
		DrawDebugSphere(GetWorld(), NextPathPoint, 20.f, 12, FColor::Blue, false, 6.f, 0, 5.f);
	}
	else
	{

		// Keep moving to target

		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();

		Mesh->AddForce(ForceDirection * MovementForce, NAME_None, bUseVelocityChange);
	}
}


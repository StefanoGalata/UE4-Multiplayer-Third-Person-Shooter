// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "GameModes/TPSGameMode.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100.f;

	SetIsReplicatedByDefault(true);
}


void UHealthComponent::Heal(float Amount)
{
	if (Amount <= 0 || CurrentHealth <= 0.f)
	{
		return;
	}
	CurrentHealth = FMath::Clamp(CurrentHealth+Amount, 0.f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health changed: %s (+%s)"), *FString::SanitizeFloat(CurrentHealth), *FString::SanitizeFloat(Amount));
	OnHealthChanged.Broadcast(this, CurrentHealth, -Amount, nullptr, nullptr, nullptr);
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only hook if we are server
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeDamage);
		}
	}

	CurrentHealth = DefaultHealth;
}

void UHealthComponent::OnRep_CurrentHealth(float OldHealth)
{
	float Damage = CurrentHealth-OldHealth;

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, nullptr, nullptr, nullptr);
}

void UHealthComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.f || bIsDead)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, DefaultHealth);

	bIsDead = CurrentHealth <= 0.f;

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);

	if (CurrentHealth <= 0)
	{
		ATPSGameMode* GM = Cast<ATPSGameMode>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
	
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}


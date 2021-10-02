// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TPSREPLICATION_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();


	FVector GetPawnViewLocation() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed = 20.f;																			 

	void BeginCrouch();
	void EndCrouch();

	void BeginZoom();
	void EndZoom();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	bool bWantsToZoom = false;

	float DefaultFOV;



};

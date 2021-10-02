// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0,0,60.f));

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm);

	ZoomedFOV = 65.f;
	DefaultFOV = 90.f;

}

FVector ATPSCharacter::GetPawnViewLocation() const
{
	if (Camera)
	{
		return Camera->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ATPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ATPSCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ATPSCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ATPSCharacter::EndZoom);
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultFOV = Camera->FieldOfView;
}

void ATPSCharacter::BeginCrouch()
{
	Crouch();
}

void ATPSCharacter::EndCrouch()
{
	UnCrouch();
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	
	Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed));
	
}

void ATPSCharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector() * Amount);
}

void ATPSCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector() * Amount);
}

void ATPSCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ATPSCharacter::EndZoom()
{
	bWantsToZoom = false;
}



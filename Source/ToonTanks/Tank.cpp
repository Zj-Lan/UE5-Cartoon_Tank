// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
}

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 100, FColor::Red);
		DrawDebugLine(GetWorld(), GetProjectileSpawnLocation(), HitResult.ImpactPoint, FColor::Red,
				false, -1, 0, 5);

		RotateTurret(HitResult.ImpactPoint);
	}

	Reloaded += ReloadRate * UGameplayStatics::GetWorldDeltaSeconds(this);
}

void ATank::Fire()
{
	if (Reloaded > 1.f)
	{
		Super::Fire();
		Reloaded = 0.f;
	}
}

void ATank::Move(float Value)
{
	MoveForward = Value;
	AddActorLocalOffset(
		FVector(Value*TankSpeed*UGameplayStatics::GetWorldDeltaSeconds(this),0.f,0.f),
		true
	);
}

void ATank::Turn(float Value)
{
	if (MoveForward<0)
	{
		Value = -Value;
	}
	
	AddActorLocalRotation(
		FRotator(0.f,Value*TurnRate*UGameplayStatics::GetWorldDeltaSeconds(this),0.f),
		true
	);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

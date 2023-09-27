// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bash Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	
	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn"));
	ProjectileSpawn->SetupAttachment(TurretMesh);
	
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::HandleDestruction()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DestroyedParticle,
		this->GetActorLocation(),
		this->GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound,
		this->GetActorLocation(),
		this->GetActorRotation());

	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
}

void ABasePawn::HandleDamage()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, GotHitParticle,
		this->GetActorLocation(),
		this->GetActorRotation());
}

void ABasePawn::RotateTurret(const FVector& Target)
{
	FRotator TargetRotation = (Target-TurretMesh->GetComponentLocation()).Rotation();
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;
	
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			TargetRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			2.f
		)
	);
}

void ABasePawn::Fire()
{
	// DrawDebugSphere(GetWorld(), ProjectileSpawn->GetComponentLocation(), 10, 10, FColor::Red, true);
	GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		ProjectileSpawn->GetComponentLocation(),
		ProjectileSpawn->GetComponentRotation()
	)->SetOwner(this);
}


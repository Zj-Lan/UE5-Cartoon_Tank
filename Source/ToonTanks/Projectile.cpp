// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;

	ProjectileParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projectile Particles"));
	ProjectileParticles->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, this->GetActorLocation(), this->GetActorRotation());
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles,
		this->GetActorLocation(),
		this->GetActorRotation());
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalUmpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner() && GetOwner())
	{
		UGameplayStatics::ApplyDamage(
			OtherActor, Damage,
			GetOwner()->GetInstigatorController(),
			this, UDamageType::StaticClass()
		);
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
	}
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, Hit.Location, this->GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.Location, this->GetActorRotation());
	
	//Destroy();
	// if (GetLifeSpan() == 0)
	{
		SetLifeSpan(1.f);
	}
	// ProjectileMesh->SetVisibility(false);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "Tank.h"
#include "Elements/Framework/TypedElementLimits.h"
#include "Kismet/GameplayStatics.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireTimer, this, &ATower::CheckFire, FMath::RandRange(FireRate*0.8, FireRate*1.2), true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckDistance())
	{
		RotateTurret(Tank->GetActorLocation());
	}
	
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATower::CheckFire()
{
	if (CheckDistance() && CheckTank())
	{
		Fire();
	}
}

bool ATower::CheckDistance() const
{
	return Tank && FVector::Distance(GetActorLocation(), Tank->GetActorLocation()) < FireRange;
}

bool ATower::CheckTank() const
{
	return Tank && Tank->GetTankPlayerController()->bShowMouseCursor;
}

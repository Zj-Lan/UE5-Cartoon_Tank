// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTankGameMode.h"

#include "Tank.h"
#include "ToonTankPlayerController.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"

void AToonTankGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTankGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTankPlayerController)
		{
			ToonTankPlayerController->SetPlayerEnabledState(false);
		}
		// Tank->DisableInput(Tank->GetTankPlayerController());
		// Tank->GetTankPlayerController()->bShowMouseCursor = false;
		GameOver(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		TargetTowers--;
		if (TargetTowers == 0)
		{
			GameOver(true);			
		}
	}
}

void AToonTankGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTankPlayerController = Cast<AToonTankPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();
	
	if (ToonTankPlayerController)
	{
		ToonTankPlayerController->SetPlayerEnabledState(false);

		FTimerHandle TimerHandle;
		const auto TimerDelegate = FTimerDelegate::CreateUObject(
							ToonTankPlayerController,
							&AToonTankPlayerController::SetPlayerEnabledState,
							true);

		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, StartDelay, false);
	}
}

int32 AToonTankGameMode::GetTargetTowerCount()
{
	TArray<AActor*> tmp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), tmp);

	return tmp.Num();
}

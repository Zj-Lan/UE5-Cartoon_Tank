// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

private:
	void CheckFire();
	bool CheckDistance() const;
	bool CheckTank() const;
	
private:
	class ATank* Tank;

	UPROPERTY(EditAnywhere, Category="Tower Stat")
	float FireRange = 700.f;

	UPROPERTY(EditAnywhere, Category="Tower Stat")
	float FireRate = 2.f;

	FTimerHandle FireTimer;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	ATank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const
	{
		return TankPlayerController;
	}

	void Fire();

private:
	void Move(float Value);
	void Turn(float Value);

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category="TankStat")
	float TankSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category="TankStat")
	float TurnRate = 50.f;

	UPROPERTY(EditAnywhere, Category="TankStat")
	float ReloadRate = 1.f;

	float Reloaded = 0.f;

	APlayerController* TankPlayerController;

	float MoveForward = 0.f;

};

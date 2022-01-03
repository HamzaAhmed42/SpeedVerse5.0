// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "VehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDVERSE_API AVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:

	AVehiclePawn();
	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Throttle(float Value);
	void Steer(float Value);
	void Turn(float Value);
	void Look(float Value);
	void BreakPressed();
	void BreakReleased();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senstivity", meta = (AllowPrivateAccess = "true"))
	float MouseSensi;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "VehiclePawn.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECamState : uint8 
{
	ECS_Front UMETA(DisplayName = "Front Pov"),
	ECS_Pit UMETA(DisplayName = "Pit Pov"),
	ECS_Spoiler UMETA(DisplayName = "Spoiler Pov"),
	ECS_Back UMETA(DisplayName = "Back Pov"),

	ECS_MAX UMETA(DisplayName = "MAX"),
};

UENUM(BlueprintType)
enum class EVehicleState : uint8 
{
	EVS_Neutral UMETA(DisplayName = "Neutral Mode"),
	ECS_Boost UMETA(DisplayName = "Boost Mode"),
	ECS_Ers UMETA(DisplayName = "Ers Mode"),

	ECS_MAX UMETA(DisplayName = "MAX"),
};


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
	

	//Function handling all Camera Rotation 
	void CameraMovement(float DeltaTime);
	
	void ChangeCamPerspective();
	//function to change our active camera
	void SwitchCamera();

	void Boosting();
	
	void GearUp();
	void GearDown();
	//set up our transmission to either manual or auto 
	void TransMission();

	void ERS();
	void ErsUpdate();

	float GetAcceleration();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senstivity", meta = (AllowPrivateAccess = "true"))
	float MouseSensi;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FrontCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PitCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* MapCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComponent;
	
	//Arrow Component to set Camera Rotation through Blueprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* PlayerRotationArrow;
	//Camera Default Rotation can be set through Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FRotator CameraDefaultRotation;
	//Speed at which camera will come to its default position
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraLerpAlpha;

	//Vehicle velocity at a given time
	FVector Velocity;
	//Vehicle Velocity last frame
	FVector VelocityLastFrame;

	//is steering button pressed
	bool bSteering;
	//is Throttle button pressed
	bool bThrottle;

	//Velocity After Which Camera will switch to non free cam mode
	float CameraInterpVelocity;

	// active Cam state Enum
	ECamState CamState;

	// Lap Box Number 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Lap, meta = (AllowPrivateAccess = "true"))
	int32 Lap;
	int32 LapTriggerBoxNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Car, meta = (AllowPrivateAccess = "true"))
	float CurrentBoostValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Car, meta = (AllowPrivateAccess = "true"))
	float FinalBoostValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Car, meta = (AllowPrivateAccess = "true"))
	bool bTransmissionAutomatic;
	
	bool bIsBoosting;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bBreakPressed;
	float BoostSpeed;
	FVector Vel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD, meta = (AllowPrivateAccess = "true"))
	int32 GearUpdate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ers", meta = (AllowPrivateAccess = true))
	bool bErsBoost;
	float MaxErs;
	bool bErsTimer;
	float Ers;
	FTimerHandle ErsTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ers", meta = (AllowPrivateAccess = true))
	float CurrentErs;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ers", meta = (AllowPrivateAccess = true))
	float ErsGainSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ers", meta = (AllowPrivateAccess = true))
	float ErsLossSpeed;
	bool bCanBoost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ers", meta = (AllowPrivateAccess = true))
	float ErsWaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ers", meta = (AllowPrivateAccess = true))
	float BoostMagnitude;
	EVehicleState VehicleState;

	float OldVelocity;
	float NewVelocity;

public:

	FORCEINLINE ECamState GetCamState(){return CamState;}
	
	FORCEINLINE int32 GetTriggerBoxNumber(){return LapTriggerBoxNumber;}
	FORCEINLINE void SetTriggerBoxNumber(int32 Value){LapTriggerBoxNumber = Value;}
	FORCEINLINE int32 GetCurrentLap(){return Lap;}
	FORCEINLINE void SetCurrentLap(){Lap++;}
		

};



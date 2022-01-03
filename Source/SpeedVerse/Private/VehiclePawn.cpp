// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"

AVehiclePawn::AVehiclePawn() :
MouseSensi(40.0f)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);

	GetMesh()->SetSimulatePhysics(true);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
}

void AVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Throttle"), this, &AVehiclePawn::Throttle);
	PlayerInputComponent->BindAxis(FName("Steer"), this, &AVehiclePawn::Steer);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AVehiclePawn::Turn);
	PlayerInputComponent->BindAxis(FName("Look"), this, &AVehiclePawn::Look);
	PlayerInputComponent->BindAction(FName("Break"), IE_Pressed,this, &AVehiclePawn::BreakPressed);
	PlayerInputComponent->BindAction(FName("Break"), IE_Released,this, &AVehiclePawn::BreakReleased);
}

void AVehiclePawn::Throttle(float Value)
{
	if(Controller)
	{
		GetVehicleMovementComponent()->SetThrottleInput(Value);
	}
}

void AVehiclePawn::Steer(float Value)
{
	if(Controller)
	{
		GetVehicleMovementComponent()->SetSteeringInput(Value);
	}
}

void AVehiclePawn::Turn(float Value)
{
	if(Controller && Value!=0)
	{
		AddControllerYawInput(Value*GetWorld()->DeltaTimeSeconds*MouseSensi);
	}
}

void AVehiclePawn::Look(float Value)
{
	if(Controller && Value!=0)
	{
		AddControllerPitchInput(Value*GetWorld()->DeltaTimeSeconds*MouseSensi);
	}
}

void AVehiclePawn::BreakPressed()
{
	if(Controller)
	{
		GetVehicleMovementComponent()->SetBrakeInput(true);
	}
}

void AVehiclePawn::BreakReleased()
{
	if(Controller)
	{
		GetVehicleMovementComponent()->SetBrakeInput(false);
	}
}

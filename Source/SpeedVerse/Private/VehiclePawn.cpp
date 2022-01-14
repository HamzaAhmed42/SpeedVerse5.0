// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

AVehiclePawn::AVehiclePawn() :
MouseSensi(40.0f),
CameraDefaultRotation(0),
bSteering(false),
bThrottle(false),
CameraLerpAlpha(0.05),
Velocity(0),
VelocityLastFrame(0),
CameraInterpVelocity(300.0f),
CamState(ECamState::ECS_Back),
//----------------Lap------------------
Lap(0),
LapTriggerBoxNumber(1),
//---------------CAR---------------
CurrentBoostValue(0.0f),
FinalBoostValue(100.0f),
bIsBoosting(false),
BoostSpeed(2.0f),
bTransmissionAutomatic(false),
GearUpdate(1),
MaxErs(100.0f),
Ers(100.0f),
ErsGainSpeed(0.002),
ErsLossSpeed(0.02),
bErsBoost(false),
bErsTimer(true),
bCanBoost(false),
ErsWaitTime(1.0f),
BoostMagnitude(500.0f)
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

	FrontCamera = CreateDefaultSubobject<UCameraComponent>("Front Camera Component");
	FrontCamera->SetupAttachment(GetRootComponent());

	PitCamera = CreateDefaultSubobject<UCameraComponent>("Pit Camera Component");
	PitCamera->SetupAttachment(GetRootComponent());
	
	MapCamera = CreateDefaultSubobject<UCameraComponent>("Map Camera");
	MapCamera->SetupAttachment(GetRootComponent());

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("Scene Capture Component");
	SceneCaptureComponent->SetupAttachment(MapCamera);
	
	PlayerRotationArrow = CreateDefaultSubobject<UArrowComponent>("Reference Arrow");
	PlayerRotationArrow->SetupAttachment(GetRootComponent());

	GetMesh()->SetSimulatePhysics(true);
}

void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	SwitchCamera();
	CurrentBoostValue = 0.0f;
	// Vel = GetMesh()->
	GetVehicleMovementComponent()->SetUseAutomaticGears(false);

	VehicleState = EVehicleState::EVS_Neutral;
}

void AVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CameraMovement(DeltaSeconds);
	Boosting();
	ERS();
	
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

	PlayerInputComponent->BindAction("ChangePerspective", IE_Pressed, this, &AVehiclePawn::ChangeCamPerspective);

	PlayerInputComponent->BindAction("GearUp", IE_Pressed, this, &AVehiclePawn::GearUp);
	PlayerInputComponent->BindAction("GearDown", IE_Pressed, this, &AVehiclePawn::GearDown);

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

void AVehiclePawn::GearUp()
{
	if(GearUpdate < 5 && GetMesh()->GetComponentVelocity().Size()> 1000)
	{
		GearUpdate++;
		TransMission();
	}
}

void AVehiclePawn::GearDown()
{
	if(GearUpdate > 1)
	{
		GearUpdate--;
		TransMission();
	}
}

void AVehiclePawn::CameraMovement(float DeltaTime)
{
	if(CamState == ECamState::ECS_Spoiler || CamState == ECamState::ECS_Back)
	{
		VelocityLastFrame = Velocity;
		Velocity = GetVelocity();
	
		//DeltaVelocity<0 = deaccelerating , deltaVelocity>0 = accelerating 
		float deltaVelocity = (Velocity.Size() - VelocityLastFrame.Size());

		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

		if(playerController == nullptr){return;}
	
		FRotator playerRotation = playerController->GetControlRotation();
		CameraDefaultRotation = PlayerRotationArrow->GetComponentRotation();

		if(!bThrottle && Velocity.Size()<CameraInterpVelocity)
		{
			Camera->bUsePawnControlRotation = true;
		}
		if(((bSteering||bThrottle) || deltaVelocity!=0) && Velocity.Size() >= CameraInterpVelocity)
		{
			Camera->bUsePawnControlRotation = false;

			//Lerping Camera to default Rotation
			FRotator lerpRotation = FMath::Lerp(playerRotation, CameraDefaultRotation, CameraLerpAlpha);
			playerController->SetControlRotation(lerpRotation);
		}
		else
		{
			Camera->bUsePawnControlRotation = true;
		}
		
	}
}

void AVehiclePawn::ChangeCamPerspective()
{
	if(CamState == ECamState::ECS_Front)
	{
		CamState = ECamState::ECS_Pit;
	}
	else if(CamState == ECamState::ECS_Pit)
	{
		CamState = ECamState::ECS_Spoiler;
	}
	else if(CamState == ECamState::ECS_Spoiler)
	{
		CamState = ECamState::ECS_Back;
	}
	else
	{
		CamState = ECamState::ECS_Front;
	}

	SwitchCamera();
}

void AVehiclePawn::SwitchCamera()
{
	if(CamState == ECamState::ECS_Front)
	{
		Camera->SetActive(false);
		PitCamera->SetActive(false);
		FrontCamera->SetActive(true);
	}
	else if(CamState == ECamState::ECS_Pit)
	{
		Camera->SetActive(false);
		PitCamera->SetActive(true);
		FrontCamera->SetActive(false);
		
	}
	else if(CamState == ECamState::ECS_Spoiler)
	{
		Camera->SetActive(true);
		PitCamera->SetActive(false);
		FrontCamera->SetActive(false);
		CameraBoom->TargetArmLength = 500.0f;
	}
	else
	{
		Camera->SetActive(true);
		PitCamera->SetActive(false);
		FrontCamera->SetActive(false);
		CameraBoom->TargetArmLength = 900.0f;
	}
}

void AVehiclePawn::TransMission()
{
	if(bTransmissionAutomatic)
	{
		GetVehicleMovementComponent()->SetUseAutomaticGears(true);
	}
	else
	{
		GetVehicleMovementComponent()->SetUseAutomaticGears(false);
		GetVehicleMovementComponent()->SetTargetGear(GearUpdate, true);
	}
}

void AVehiclePawn::ERS()
{

	FVector VehicleVelocity = GetMesh()->GetComponentVelocity();
	VehicleVelocity.Z = 0.0f;
	float Speed = VehicleVelocity.Size();

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3, -1, FColor::Green, FString::Printf(TEXT("Speed : %f"), Speed));
	}
	
	if(GetAcceleration()<0)
	{
		if(bErsTimer)
		{
			bErsTimer = false;
			VehicleState = EVehicleState::EVS_Neutral;
			GetWorldTimerManager().SetTimer(ErsTimerHandle, this, &AVehiclePawn::ErsUpdate, ErsWaitTime);
		}
		
		if(bErsBoost && !bIsBoosting && Speed > 500.0f)
		{
			CurrentErs = FMath::Lerp(Ers, MaxErs, ErsGainSpeed);
			Ers = CurrentErs;
			VehicleState = EVehicleState::ECS_Ers;
		}
	}
	else if(bIsBoosting)
	{
		CurrentErs = FMath::Lerp(Ers, 0.0f, ErsLossSpeed);
		Ers = CurrentErs;
		VehicleState = EVehicleState::ECS_Boost;
	}
	else if(GetAcceleration()>0)
	{
		bErsBoost = false;
	}

	if(CurrentErs>3)
	{
		bCanBoost = true;
	}
	else
	{
		bCanBoost = false;
		bIsBoosting = false;
	}
}

void AVehiclePawn::ErsUpdate()
{
	bErsBoost = true;
	bErsTimer = true;
	GetWorldTimerManager().ClearTimer(ErsTimerHandle);
}

float AVehiclePawn::GetAcceleration()
{
	OldVelocity = NewVelocity;
	NewVelocity = GetMesh()->GetComponentVelocity().Size();

	return NewVelocity - OldVelocity;
}

void AVehiclePawn::Boosting()
{
	
	// if(!bIsBoosting)
	// {
	// 	float initialBoostValue = CurrentBoostValue;
	// 	CurrentBoostValue = FMath::FInterpTo(initialBoostValue, FinalBoostValue, GetWorld()->GetDeltaSeconds(), BoostSpeed);
	// 	UE_LOG(LogTemp, Warning, TEXT("Shift Not Pressed : %f"), CurrentBoostValue);
	// }
	// UE_LOG(LogTemp, Warning, TEXT("Shift Pressed : %f"), CurrentBoostValue);
	TArray<UCameraComponent*> CameraComponentArray;
	GetComponents<UCameraComponent>(CameraComponentArray);
	for(int32 i =0; i<CameraComponentArray.Num(); i++)
	{
		if(CameraComponentArray[i]->IsActive())
		{
			if(Camera)
				if(bIsBoosting && bCanBoost)
				{
					//TODO : Max rpm  GetVehicleMovementComponent()->MaxRPM +=100.0f;
					float BoostFOV = FMath::FInterpTo(CameraComponentArray[i]->FieldOfView, 110, GetWorld()->GetDeltaSeconds(), 2);
					CameraComponentArray[i]->SetFieldOfView(BoostFOV);
					VehicleState = EVehicleState::ECS_Boost;
				}
				else
				{
					float BoostFOV = FMath::FInterpTo(CameraComponentArray[i]->FieldOfView, 90, GetWorld()->GetDeltaSeconds(), 2);
					CameraComponentArray[i]->SetFieldOfView(BoostFOV);
					VehicleState = EVehicleState::ECS_Ers;
				}
		}
	}
	
}


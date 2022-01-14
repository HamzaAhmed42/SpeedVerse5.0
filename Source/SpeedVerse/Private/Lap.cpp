// Fill out your copyright notice in the Description page of Project Settings.


#include "Lap.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALap::ALap():
LapTriggerBoxPosition(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LapTriggerBox = CreateDefaultSubobject<UBoxComponent>("Lap Trigger Box");
	LapTriggerBox->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ALap::BeginPlay()
{
	Super::BeginPlay();

	LapTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALap::OnComponentOverlap);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), LapCounterClass, LapTriggerBoxArray);
	TotalLapTriggerBoxCount = LapTriggerBoxArray.Num();

	VehiclePawn = Cast<AVehiclePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void ALap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, -1, FColor::Red, FString::Printf(TEXT("Total Laps : %i"), VehiclePawn->GetCurrentLap()));
		GEngine->AddOnScreenDebugMessage(2, -1, FColor::Red, FString::Printf(TEXT("TriggerBoxNumber : %i"), VehiclePawn->GetTriggerBoxNumber()));
		// GEngine->AddOnScreenDebugMessage(3, -1, FColor::Red, FString::Printf(TEXT(" : %i"), VehiclePawn->GetCurrentLap()));
	}
	
}

void ALap::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && VehiclePawn && LapCounterClass)
	{
		if(VehiclePawn->GetTriggerBoxNumber() == TotalLapTriggerBoxCount +1)
		{
			VehiclePawn->SetCurrentLap();
			VehiclePawn->SetTriggerBoxNumber(2);
		}
		else if(VehiclePawn->GetTriggerBoxNumber() == LapTriggerBoxPosition)
		{
			VehiclePawn->SetTriggerBoxNumber(LapTriggerBoxPosition+1);
		}	
	}
}



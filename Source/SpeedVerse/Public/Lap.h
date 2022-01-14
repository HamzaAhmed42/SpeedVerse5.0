// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehiclePawn.h"
#include "GameFramework/Actor.h"
#include "Lap.generated.h"

class UBoxComponent;
UCLASS()
class SPEEDVERSE_API ALap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult & SweepResult);

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* LapTriggerBox;

	int32 TotalLapTriggerBoxCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ALap> LapCounterClass;
	
	TArray<AActor*> LapTriggerBoxArray;

	AVehiclePawn* VehiclePawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Lap, meta=(AllowPrivateAccess = "true"))
	int32 LapTriggerBoxPosition;

};

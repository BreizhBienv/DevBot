// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JackalInteractiveProp.generated.h"


UENUM(BlueprintType)
enum class EInteractivePropType : uint8
{
	JackalRobot			UMETA(DisplayName = "JackalRobot"),
	WorkStation			UMETA(DisplayName = "WorkStation"),
	ChargingStation		UMETA(DisplayName = "ChargingStation"),
	JackalTarget	    UMETA(DisplayName = "JackalTarget"),
	PointOfInterest	    UMETA(DisplayName = "PointOfInterest"),
	Unknown			    UMETA(DisplayName = "Unknown")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnTaskFinished,
	float, RewardObtained
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnPropDetected
);

UCLASS()
class BERYL_API AJackalInteractiveProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJackalInteractiveProp();

	UPROPERTY(BlueprintAssignable)
	FOnTaskFinished OnTaskFinished;

	UPROPERTY(BlueprintAssignable)
	FOnPropDetected OnPropDetected;

	UFUNCTION(BlueprintCallable)
	void FinishPropTask();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetReward();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EInteractivePropType GetPropType();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EInteractivePropType PropType = EInteractivePropType::Unknown;

	// Eventual robot reward for completing task
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Reward = 0.f;

};

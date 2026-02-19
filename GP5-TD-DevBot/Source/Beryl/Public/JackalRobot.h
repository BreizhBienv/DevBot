// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CommsComponent.h"
#include "Components/SensorComponent.h"
#include "Components/SpatialMemoryComponent.h"
#include "JackalRobot.generated.h"


UENUM(BlueprintType)
enum class EBatteryState : uint8
{
    BatteryFull      UMETA(DisplayName = "Full"),
    BatteryHigh      UMETA(DisplayName = "High"),
    BatteryLow       UMETA(DisplayName = "Low"),
    BatteryCritical  UMETA(DisplayName = "Critical"),
    BatteryEmpty     UMETA(DisplayName = "Empty")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnJackalPowerChanged,
    bool, bIsJackalOn
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOnJackalBatteryFull
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOnJackalBatteryHigh
);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOnJackalBatteryLow
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOnJackalBatteryCritical
);

UCLASS()
class BERYL_API AJackalRobot : public APawn
{
	GENERATED_BODY()
private:
    void InterpolateWheelForces(float DeltaSeconds);

    void ProcessDesiredVelocity();


    static int InstantiatedJackals;


public:
	// Sets default values for this pawn's properties
	AJackalRobot();

    UPROPERTY(BlueprintAssignable, Category="Vehicle|Events")
    FOnJackalPowerChanged OnPowerChanged;

    UPROPERTY(BlueprintAssignable, Category = "Vehicle|Events")
    FOnJackalBatteryFull OnBatteryFull;

    UPROPERTY(BlueprintAssignable, Category = "Vehicle|Events")
    FOnJackalBatteryHigh OnBatteryHigh;

    UPROPERTY(BlueprintAssignable, Category = "Vehicle|Events")
    FOnJackalBatteryLow OnBatteryLow;

    UPROPERTY(BlueprintAssignable, Category = "Vehicle|Events")
    FOnJackalBatteryCritical OnBatteryCritical;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Battery")
    EBatteryState BatteryState = EBatteryState::BatteryFull;

    UFUNCTION(BlueprintCallable)
    UStaticMeshComponent* GetBodyMesh();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    float GetForwardSpeed();

    // Root body 
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* BodyMesh;

    // Comms
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCommsComponent* CommsComponent;

    // Sensors (LiDAR, Webcam)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USensorComponent* SensorComponent;

    // Map memory component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpatialMemoryComponent* MapMemoryComponent;

    // Wheels
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* WheelFrontLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* WheelFrontRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* WheelRearLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* WheelRearRight;

    // Distance between the wheels/width of the vehicle (cm)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Movement")
    float WheelBase = 38.f;

    // Radius of each wheel (cm)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Movement")
    float WheelRadius = 9.7f;

    // Time to max speed (s)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Movement")
    float TimeToMaxSpeed = 0.25f; 

    //  Input values [-1, 1]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Movement")
    float LeftWheelsInput = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Movement")
    float RightWheelsInput = 0.f;

    // Speed of the vehicle (cm/s)
    UPROPERTY(BlueprintReadOnly)
    float LinearSpeed;

    // Yaw angular speed of the vehicle (deg/s)
    UPROPERTY(BlueprintReadOnly)
    float YawAngularSpeed = 0.f;

    // Wheels Spinning Rate
    UPROPERTY(BlueprintReadOnly)
    float LeftWheelsCurrentSpeed = 0.f;

    UPROPERTY(BlueprintReadOnly)
    float RightWheelsCurrentSpeed = 0.f;

    // Speed multipliers
    UPROPERTY(BlueprintReadOnly)
    float LinearSpeedMultiplier;
    UPROPERTY(BlueprintReadOnly)
    float AngularSpeedMultiplier;

    // Maximum movement speed (cm/s)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Movement")
    float MaxLinearSpeed = 200.0f;

    // Maximum rotation speed (deg/s)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Movement")
    float MaxYawAngularSpeed = 230.0f;

    // Maximum payload weight (kg) 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Payload")
    float MaximumPayload = 20.0f;

    // Total battery capacity (Wh)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Battery")
    float BatteryCapacity = 270.0f;

    // Total battery capacity (Wh)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Battery")
    float BatteryLevel = 270.0f;

    // Battery recharge speed (Wh/s) 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Battery")
    float BatteryChargeSpeed = 0.01875f;

    // Battery consumption speed at full power (Wh/s)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Battery")
    float MaxBatteryConsumption = 0.03125f;

    // Minimum battery consumption when vehicle is on (Wh/s)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Battery")
    float MinBatteryConsumption = 0.009875f;

    // Minimum battery consumption when vehicle is on (Wh/s)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Battery")
    float BatteryConsumptionMultiplier = 1.f;

    // State (on/off) of the jackal
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Battery")
    bool bIsJackalOn = false;

    UPROPERTY(BlueprintReadOnly)
    float LeftWheelsCurrentForce = 0.f;

    UPROPERTY(BlueprintReadOnly)
    float RightWheelsCurrentForce = 0.f;

    UPROPERTY(BlueprintReadOnly)
    bool bIsOnFloor = false;


    // Vehicle controls
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Control")
    bool bUseDesiredVelocity = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Control")
    float DesiredLinearSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Control")
    float DesiredAngularSpeed;

    // Is jackal currently completing a task
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jackal|Tasks")
    bool bIsWorking = false;

private:
    // Check if wheel is grounded
    bool IsGrounded(UStaticMeshComponent* Component, float TraceLength = 25.f);

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Called in tick to compute physics
    void ComputeDrivePhysics(float DeltaSeconds);

    // Called in tick to display wheels turning
    void SpinWheelsAnimation(float DeltaSeconds);

    // Update linear and angular speed values
    void UpdateSpeedValues();

    // Update battery
    void UpdateBatteryConsumption(float DeltaSeconds);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Check if all wheels are grounded
    bool AreAllWheelsGrounded(float TraceLength = 15.f);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Jackal|Tasks")
    bool GetIsWorking();

    UFUNCTION(BlueprintCallable, Category = "Jackal|Tasks")
    void SetIsWorking(bool bNewIsWorking);

    UFUNCTION(BlueprintCallable, Category="Jackal")
    // Turn Jackal On or Off
    void SetJackalPower(bool bNewIsJackalOn);

    UFUNCTION(BlueprintCallable, Category="Jackal|Battery")
    void RechargeBattery(float DeltaSeconds);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int GetJackalId();
};



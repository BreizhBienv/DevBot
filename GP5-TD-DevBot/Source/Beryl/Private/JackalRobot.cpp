// Fill out your copyright notice in the Description page of Project Settings.


#include "JackalRobot.h"


// Sets default values
AJackalRobot::AJackalRobot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CommsComponent = CreateDefaultSubobject<UCommsComponent>(TEXT("Comms"));
    MapMemoryComponent = CreateDefaultSubobject<USpatialMemoryComponent>(TEXT("RobotMap"));
}

UStaticMeshComponent* AJackalRobot::GetBodyMesh()
{
    return BodyMesh;
}

// Called when the game starts or when spawned
void AJackalRobot::BeginPlay()
{
	Super::BeginPlay();

    // Set Jackal physical properties
    BodyMesh->SetCenterOfMass(FVector(0.f, 0.f, 15.f));
    BodyMesh->SetAngularDamping(1.75);
    BodyMesh->SetLinearDamping(1.8);
    LinearSpeedMultiplier = 4000.f;
    AngularSpeedMultiplier = 45.f;


    FBodyInstance* BI = BodyMesh->GetBodyInstance();
    if (BI)
    {
        BI->InertiaTensorScale = FVector(2.f, 2.f, 2.5f);
        BI->UpdateMassProperties();
    }

    // TD>>>
    if (SensorComponent && MapMemoryComponent)
    {
        SensorComponent->OnScanCompleted.AddDynamic(MapMemoryComponent, &USpatialMemoryComponent::StoreDetectedActorsArray);
    }
    // <<<TD
}

// Called every frame
void AJackalRobot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateSpeedValues();
    InterpolateWheelForces(DeltaTime);

    if (bIsJackalOn)
    {
        if (bUseDesiredVelocity) ProcessDesiredVelocity();
        // Cannot drive while working
        if (!bIsWorking)
        {
            ComputeDrivePhysics(DeltaTime);
            SpinWheelsAnimation(DeltaTime);
        }
        UpdateBatteryConsumption(DeltaTime);
    }
}

// Called to bind functionality to input
void AJackalRobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AJackalRobot::InterpolateWheelForces(float DeltaSeconds)
{
    // Optional: Interpolate input to forces to smooth acceleration
    RightWheelsCurrentForce = FMath::Clamp(RightWheelsInput, -1.f, 1.f);
    LeftWheelsCurrentForce = FMath::Clamp(LeftWheelsInput, -1.f, 1.f);
}

void AJackalRobot::ProcessDesiredVelocity()
{
    const float DesiredLinearForce = DesiredLinearSpeed / MaxLinearSpeed;
    const float DesiredAngularForce = DesiredAngularSpeed / MaxYawAngularSpeed;

    LeftWheelsInput = FMath::Clamp(DesiredAngularForce + DesiredLinearForce, -1.f, 1.f);
    RightWheelsInput = FMath::Clamp(-DesiredAngularForce + DesiredLinearForce, -1.f, 1.f);
}


void AJackalRobot::ComputeDrivePhysics(float DeltaSeconds)
{
    if (!BodyMesh || !BodyMesh->IsSimulatingPhysics())
        return;
    
    LeftWheelsCurrentSpeed = LeftWheelsCurrentForce * WheelRadius;
    RightWheelsCurrentSpeed = RightWheelsCurrentForce * WheelRadius;

    int GroundedCounter = 0;
    const bool bFrontLeftGrounded = IsGrounded(WheelFrontLeft);
    const bool bFrontRightGrounded = IsGrounded(WheelFrontRight);
    const bool bRearLeftGrounded = IsGrounded(WheelRearLeft);
    const bool bRearRightGrounded = IsGrounded(WheelRearRight);

    bIsOnFloor = bFrontLeftGrounded || bFrontRightGrounded || bRearLeftGrounded || bRearRightGrounded;

    if (bIsOnFloor)
    {
        FVector Forward = BodyMesh->GetForwardVector();

        const float LeftTractionForce = 
              bFrontLeftGrounded ? LeftWheelsCurrentSpeed / 2.0 : 0.f 
            + bRearLeftGrounded  ? LeftWheelsCurrentSpeed / 2.0 : 0.f;
        const float RightTractionForce = 
              bFrontRightGrounded ? RightWheelsCurrentSpeed / 2.0 : 0.f
            + bRearRightGrounded  ? RightWheelsCurrentSpeed / 2.0 : 0.f;

        const float TargetLinearSpeed = (LeftWheelsCurrentSpeed + RightWheelsCurrentSpeed) * 0.5f;
        const float TargetAngularSpeed = (RightWheelsCurrentSpeed - LeftWheelsCurrentSpeed) / WheelBase;

        // Linear velocity
        BodyMesh->AddForce(
            Forward * TargetLinearSpeed * LinearSpeedMultiplier
        );

        // Skid-steer rotation
        BodyMesh->AddTorqueInRadians(
            FVector(0.f, 0.f, TargetAngularSpeed * AngularSpeedMultiplier),
            NAME_None,
            true
        );
    }
}

bool AJackalRobot::AreAllWheelsGrounded(float TraceLength)
{
    return     IsGrounded(WheelFrontLeft, TraceLength)
            && IsGrounded(WheelFrontRight, TraceLength)
            && IsGrounded(WheelRearLeft, TraceLength)
            && IsGrounded(WheelRearRight, TraceLength);
}

bool AJackalRobot::GetIsWorking()
{
    return bIsWorking;
}

void AJackalRobot::SetIsWorking(bool bNewIsWorking)
{
    if (bIsJackalOn) {
        bIsWorking = bNewIsWorking;
    }
    else
    {
        bIsWorking = false;
    }
}

void AJackalRobot::UpdateSpeedValues()
{
    if (!BodyMesh)
        return;
    
    LinearSpeed = FVector::DotProduct(BodyMesh->GetPhysicsLinearVelocity(), BodyMesh->GetForwardVector()); //FVector::Dist(PreviousBodyLocation, NewBodyLocation) / DeltaSeconds;
    YawAngularSpeed = BodyMesh->GetPhysicsAngularVelocityInDegrees().Z; //FMath::Abs(PreviousBodyYawRotation - NewBodyYawRotation) / DeltaSeconds;
}

void AJackalRobot::UpdateBatteryConsumption(float DeltaSeconds)
{
    // When working, use max amount of battery
    if (bIsWorking)
    {
        BatteryLevel -= BatteryConsumptionMultiplier * MaxBatteryConsumption * DeltaSeconds;
    } 
    // When driving, depend on motors speed
    else
    {
        BatteryLevel -= BatteryConsumptionMultiplier * FMath::Max(
            MinBatteryConsumption * DeltaSeconds,
            MaxBatteryConsumption * (FMath::Abs(LeftWheelsInput) + FMath::Abs(RightWheelsInput)) / 2.0 * DeltaSeconds
        );
    }
    
    // Update battery states & warnings
    if (BatteryLevel > BatteryCapacity * 0.9f)
    {
        // Battery full
        if (BatteryState != EBatteryState::BatteryFull)
        {
            BatteryState = EBatteryState::BatteryFull;
            OnBatteryFull.Broadcast();
        }
    }
    else if (BatteryLevel > BatteryCapacity * 0.4f)
    {
        // Battery high
        if (BatteryState != EBatteryState::BatteryHigh) 
        {
            BatteryState = EBatteryState::BatteryHigh;
            OnBatteryHigh.Broadcast();
        }
    } 
    else if (BatteryLevel > BatteryCapacity * 0.2f)
    {
        // Battery low
        if (BatteryState != EBatteryState::BatteryLow)
        {
            BatteryState = EBatteryState::BatteryLow;
            OnBatteryLow.Broadcast();
        }
    }
    else if (BatteryLevel > 0.f)
    {
        // Battery critical
        if (BatteryState != EBatteryState::BatteryCritical)
        {
            BatteryState = EBatteryState::BatteryCritical;
            OnBatteryCritical.Broadcast();
        }
    }
    else
    {
        // Battery empty
        BatteryLevel = 0.f;
        BatteryState = EBatteryState::BatteryEmpty;
        SetJackalPower(false);
    }
}

void AJackalRobot::SetJackalPower(bool bNewIsJackalOn)
{
    if (bNewIsJackalOn == bIsJackalOn) return;
    if (bNewIsJackalOn && BatteryLevel <= 0.f) return;

    bIsJackalOn = bNewIsJackalOn;
    if (!bIsJackalOn)
    {
        bIsWorking = false;
        RightWheelsCurrentForce = 0.f;
        LeftWheelsCurrentForce = 0.f;
    }
    OnPowerChanged.Broadcast(bIsJackalOn);
}

void AJackalRobot::RechargeBattery(float DeltaSeconds)
{
    BatteryLevel += DeltaSeconds * BatteryChargeSpeed;
    if (BatteryLevel > BatteryCapacity) BatteryLevel = BatteryCapacity;
}

int AJackalRobot::GetJackalId()
{
    return GetUniqueID();
}


void AJackalRobot::SpinWheelsAnimation(float DeltaSeconds)
{

    const FRotator LeftDeltaRotation((LeftWheelsCurrentSpeed / WheelRadius) * 360.f * DeltaSeconds, 0.f, 0.f);

    WheelFrontLeft->AddLocalRotation(LeftDeltaRotation);
    WheelRearLeft->AddLocalRotation(LeftDeltaRotation);

    const FRotator RightDeltaRotation((RightWheelsCurrentSpeed / WheelRadius) * 360.f * DeltaSeconds, 0.f, 0.f);
    WheelFrontRight->AddLocalRotation(RightDeltaRotation);
    WheelRearRight->AddLocalRotation(RightDeltaRotation);
}

bool AJackalRobot::IsGrounded(UStaticMeshComponent* Component, float TraceLength)
{
    FVector Start = Component->GetComponentLocation();
    FVector End = Start - FVector(0, 0, TraceLength);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    return GetWorld()->LineTraceSingleByChannel(
        Hit, Start, End, ECC_Visibility, Params
    );
}

float AJackalRobot::GetForwardSpeed()
{
    const FVector Velocity = BodyMesh->GetPhysicsLinearVelocity();
    return FVector::DotProduct(Velocity, BodyMesh->GetForwardVector());
}

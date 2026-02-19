#include "Components/SensorComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

USensorComponent::USensorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.25f;
}

void USensorComponent::BeginPlay()
{
    Super::BeginPlay();
}


void USensorComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (ScanOnTick)
    {
        ScanEnvironment();
    }
}


TArray<AActor*> USensorComponent::ScanEnvironment()
{
    TArray<AActor*> Results;

    UWorld* World = GetWorld();
    if (!World) return Results;

    
    FVector Origin = GetComponentLocation();
    FRotator BaseRot = GetComponentRotation();

    float HalfAngle = DetectionAngle * 0.5f;

    for (int32 i = 0; i < RayCount; i++)
    {
        float Alpha = (float)i / (RayCount - 1);
        float AngleOffset = FMath::Lerp(-HalfAngle, HalfAngle, Alpha);

        FRotator RayRot = BaseRot + FRotator(0.f, AngleOffset, 0.f);
        FVector End = Origin + RayRot.Vector() * DetectionRange;

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(GetOwner());

        if (World->LineTraceSingleByChannel(
            Hit,
            Origin,
            End,
            ECC_Visibility,
            Params))
        {
            AActor* NewActorDetected = Hit.GetActor();

            // Filter duplicates
            if (Results.Contains(NewActorDetected))
                continue;

            if (AJackalInteractiveProp* DetectedInteractiveProp = Cast<AJackalInteractiveProp>(NewActorDetected))
            {
                DetectedInteractiveProp->OnPropDetected.Broadcast();
            }
            Results.Add(NewActorDetected);
        }

        // Debug ray
        if (DrawDebugRaycasts)
        {
            DrawDebugLine(
                World,
                Origin,
                End,
                Hit.bBlockingHit ? FColor::Red : FColor::Green,
                false,
                ScanDebugDuration,
                0,
                0.5f
            );
        }
    }

    OnScanCompleted.Broadcast(Results);
    return Results;
}

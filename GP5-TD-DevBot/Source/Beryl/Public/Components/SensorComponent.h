#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Props/JackalInteractiveProp.h"
#include "SensorComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnScanCompleted,
    TArray<AActor*>, ScanResults
);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BERYL_API USensorComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    USensorComponent();


    UPROPERTY(BlueprintAssignable, Category = "Vehicle|Events")
    FOnScanCompleted OnScanCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    float DetectionAngle = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    float DetectionRange = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    int32 RayCount = 25;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    bool ScanOnTick = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    bool DrawDebugRaycasts = true;

    UFUNCTION(BlueprintCallable, Category = "Sensor")
    TArray<AActor*> ScanEnvironment();

protected:
    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY()
    float ScanDebugDuration = 0.25f;
};

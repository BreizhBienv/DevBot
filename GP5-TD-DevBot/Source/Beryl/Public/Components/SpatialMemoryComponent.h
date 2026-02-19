#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Props/JackalInteractiveProp.h"
#include "SpatialMemoryComponent.generated.h"

class AJackalRobot;


USTRUCT(BlueprintType)
struct FSpatialGridCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector CellCoordinates = FIntVector::ZeroValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCellExplored = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> CellPropIDs;
};

USTRUCT(BlueprintType)
struct FSpatialMemoryElement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EInteractivePropType DetectedObject = EInteractivePropType::Unknown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ElementReward = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ElementID = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Timestamp = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnMapUpdated
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BERYL_API USpatialMemoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpatialMemoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "SpatialMemory")
	FOnMapUpdated OnMapUpdated;

	UFUNCTION(BlueprintCallable, Category = "SpatialMemory|Objects")
	void AddSpatialMemoryElement(FSpatialMemoryElement NewElement);

	UFUNCTION(BlueprintCallable, Category = "SpatialMemory|Objects")
	void StoreDetectedActorsArray(TArray<AActor*> DetectedActorsArray);

	UFUNCTION(BlueprintCallable, Category = "SpatialMemory|Objects")
	void StoreDetectedActor(AActor* DetectedActor);

	UFUNCTION(BlueprintCallable, Category = "SpatialMemory|Objects")
	void MergeSpatialMemoryMaps(TArray<FSpatialMemoryElement> OtherMap);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SpatialMemory|Objects")
	TArray<FSpatialMemoryElement> GetSpatialItemsMemory();

	UFUNCTION(BlueprintCallable)
	void ExploreCell(FIntVector CellID);

	UFUNCTION(BlueprintCallable)
	FIntVector GetCurrentCell();

	UFUNCTION(BlueprintCallable)
	TArray<FSpatialGridCell> GetSpatialGrid();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpatialMemory|Objects")
	TArray<FSpatialMemoryElement> SpatialItemsMemory;

	UPROPERTY(EditDefaultsOnly)
	FIntVector CellSize = FIntVector(500, 500, 100); // Default cell size is 5 by 5 by 1 meters

	UPROPERTY(BlueprintReadOnly)
	TArray<FSpatialGridCell> SpatialGrid;

};

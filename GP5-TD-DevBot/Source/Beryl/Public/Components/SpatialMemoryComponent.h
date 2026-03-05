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
	bool bIsNavigable = false;

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
	void AddSpatialMemoryElement(int Id, FSpatialMemoryElement NewElement);

	UFUNCTION(BlueprintCallable, Category = "SpatialMemory|Objects")
	void StoreDetectedActorsArray(TArray<AActor*> DetectedActorsArray);

	UFUNCTION(BlueprintCallable, Category = "SpatialMemory|Objects")
	void StoreDetectedActor(AActor* DetectedActor);

	UFUNCTION(BlueprintCallable, Category = "SpatialMemory|Objects")
	void MergeSpatialMemoryMaps(TMap<int, FSpatialMemoryElement> OtherMap);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SpatialMemory|Objects")
	TMap<int, FSpatialMemoryElement> GetSpatialItemsMemory();
	
	UFUNCTION(BlueprintCallable)
	void AddCell(FIntVector CellID);
	
	UFUNCTION(BlueprintCallable)
	void GenerateAdjacentCells(FIntVector OriginCellID);
	
	UFUNCTION(BlueprintCallable)
	bool IsCellExploredWorldCoord(FVector worldCoord);

	UFUNCTION(BlueprintCallable)
	bool IsCellExplored(FIntVector CellID);
	
	UFUNCTION(BlueprintCallable)
	void ExploreCell(FIntVector CellID);

	UFUNCTION(BlueprintCallable)
	FIntVector GetCellID(FVector worldCoordinate);
	
	UFUNCTION(BlueprintCallable)
	FIntVector GetCurrentCellID();

	UFUNCTION(BlueprintCallable)
	TArray<FSpatialGridCell> GetSpatialGrid();
	
	UFUNCTION(BlueprintCallable)
	int GetCellArrayID(FIntVector CellID);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector FromCellToWorldCoord(FIntVector CellId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntVector GetCellSize();
	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpatialMemory|Objects")
	TMap<int, FSpatialMemoryElement> SpatialItemsMemory;

	UPROPERTY(EditDefaultsOnly)
	FIntVector CellSize = FIntVector(500, 500, 100); // Default cell size is 5 by 5 by 1 meters

	UPROPERTY(EditDefaultsOnly)
	FVector WorldPosOffset = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadOnly)
	TArray<FSpatialGridCell> SpatialGrid;

};

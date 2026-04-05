// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpatialMemoryComponent.h"
#include "JackalRobot.h"
#include "NavigationSystem.h"


// Sets default values for this component's properties
USpatialMemoryComponent::USpatialMemoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USpatialMemoryComponent::AddSpatialMemoryElement(int Id, FSpatialMemoryElement NewElement)
{
	SpatialItemsMemory.Add(Id, NewElement);
}

void USpatialMemoryComponent::StoreDetectedActorsArray(TArray<AActor*> DetectedActorsArray)
{
	for (int i = 0; i < DetectedActorsArray.Num(); i++)
	{
		StoreDetectedActor(DetectedActorsArray[i]);
	}
}

void USpatialMemoryComponent::StoreDetectedActor(AActor* DetectedActor)
{
	FSpatialMemoryElement NewElement;
	NewElement.Timestamp = GetWorld()->GetTimeSeconds();
	int id = DetectedActor->GetUniqueID();

	if (AJackalRobot* DetectedJackalRobot = Cast<AJackalRobot>(DetectedActor))
	{
		NewElement.ElementReward = 0.0f;
		NewElement.DetectedObject = EInteractivePropType::JackalRobot;
		NewElement.Location = DetectedJackalRobot->GetBodyMesh()->GetComponentLocation();
	}
	else if (AJackalInteractiveProp* DetectedInteractiveProp = Cast<AJackalInteractiveProp>(DetectedActor))
	{
		NewElement.ElementReward = DetectedInteractiveProp->GetReward();
		NewElement.DetectedObject = DetectedInteractiveProp->GetPropType();
		NewElement.Location = DetectedInteractiveProp->GetActorLocation();
	}

	if (NewElement.DetectedObject == EInteractivePropType::Unknown)
		return;
	
	AddSpatialMemoryElement(id, NewElement);
}

void USpatialMemoryComponent::MergeSpatialMemoryMaps(const TMap<int, FSpatialMemoryElement>& OtherMap)
{
	int itemAdded = 0, itemUpdated = 0;
	for (auto elem : OtherMap)
	{
		if (SpatialItemsMemory.Contains(elem.Key))
		{
			if (elem.Value.Timestamp < SpatialItemsMemory[elem.Key].Timestamp)
			{
				AddSpatialMemoryElement(elem.Key, elem.Value);
				++itemUpdated;
			}
		}
		else
		{
			AddSpatialMemoryElement(elem.Key, elem.Value);
			++itemAdded;
		}
	}
	
	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Got %d items and updated %d"), itemAdded, itemUpdated));
}

void USpatialMemoryComponent::MergeSpatialGrid(const TArray<FSpatialGridCell>& otherMap)
{
	int cellAdded = 0;
	for (FSpatialGridCell cell : otherMap)
	{
		if (GetCellArrayID(cell.CellCoordinates) == -1)
		{
			SpatialGrid.Add(cell);
			++cellAdded;
		}
	}
	
	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Got %d cells "), cellAdded));
}

TMap<int, FSpatialMemoryElement> USpatialMemoryComponent::GetSpatialItemsMemory()
{
	return SpatialItemsMemory;
}

void USpatialMemoryComponent::AddCell(FIntVector CellID)
{
	int arrayID = GetCellArrayID(CellID);
	if (arrayID >= 0)
		return; //Cell already exists
	
	FVector worldCell = FromCellToWorldCoord(CellID);
	FVector outNavigablePos;
	bool navigable = UNavigationSystemV1::K2_ProjectPointToNavigation(
		GetWorld(), worldCell, outNavigablePos, nullptr, {}, FVector(CellSize) / 2.f);
	
	FSpatialGridCell newCell;
	newCell.CellCoordinates = CellID;
	newCell.bIsCellExplored = false;
	newCell.bIsNavigable = navigable;
	
	SpatialGrid.Add(newCell);
}

void USpatialMemoryComponent::GenerateAdjacentCells(FIntVector OriginCellID)
{
	TArray AdjacentCellIDs = {
		//Sides
		FIntVector(OriginCellID.X + 1, OriginCellID.Y, OriginCellID.Z),
		FIntVector(OriginCellID.X - 1, OriginCellID.Y, OriginCellID.Z),
		FIntVector(OriginCellID.X, OriginCellID.Y + 1, OriginCellID.Z),
		FIntVector(OriginCellID.X, OriginCellID.Y - 1, OriginCellID.Z),
		//Above and below
		FIntVector(OriginCellID.X, OriginCellID.Y, OriginCellID.Z + 1),
		FIntVector(OriginCellID.X, OriginCellID.Y, OriginCellID.Z - 1),
		//Corners
		FIntVector(OriginCellID.X + 1, OriginCellID.Y + 1, OriginCellID.Z),
		FIntVector(OriginCellID.X + 1, OriginCellID.Y - 1, OriginCellID.Z),
		FIntVector(OriginCellID.X - 1, OriginCellID.Y - 1, OriginCellID.Z),
		FIntVector(OriginCellID.X - 1, OriginCellID.Y + 1, OriginCellID.Z),
	};
	
	for (FIntVector AdjacentCellID : AdjacentCellIDs)
		AddCell(AdjacentCellID);
}

bool USpatialMemoryComponent::IsCellExploredWorldCoord(FVector worldCoord)
{
	return IsCellExplored(GetCellID(worldCoord));
}

bool USpatialMemoryComponent::IsCellExplored(FIntVector CellID)
{
	int arrayID = GetCellArrayID(CellID);
	if (arrayID == -1)
	{
		// if(GEngine)
		// 	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, TEXT("Tried to access non registered Cell!"));
		return false;
	}
	
	return SpatialGrid[arrayID].bIsCellExplored;
}

void USpatialMemoryComponent::ExploreCell(FIntVector CellID)
{
	int arrayID = GetCellArrayID(CellID);
	if (arrayID == -1)
		return;
	
	SpatialGrid[arrayID].bIsCellExplored = true;
	
	GenerateAdjacentCells(CellID);
	OnMapUpdated.Broadcast();
}

FIntVector USpatialMemoryComponent::GetCellID(FVector worldCoordinate)
{
	return FIntVector(
		FMath::FloorToInt((worldCoordinate.X + WorldPosOffset.X) / CellSize.X),
		FMath::FloorToInt((worldCoordinate.Y + WorldPosOffset.Y) / CellSize.Y),
		FMath::FloorToInt((worldCoordinate.Z + WorldPosOffset.Z) / CellSize.Z)
		);
}

FIntVector USpatialMemoryComponent::GetCurrentCellID()
{
	return GetCellID(Cast<AJackalRobot>(GetOwner())->GetBodyMesh()->GetComponentLocation());
}

TArray<FSpatialGridCell> USpatialMemoryComponent::GetSpatialGrid()
{
	return SpatialGrid;
}

int USpatialMemoryComponent::GetCellArrayID(const FIntVector& CellID)
{	
	int id = SpatialGrid.IndexOfByPredicate(
		[CellID](const FSpatialGridCell& InItem)
		{
			return InItem.CellCoordinates == CellID;
		});
	return id == INDEX_NONE ? -1 : id;
	
	// for (int i = 0; i < SpatialGrid.Num(); i++)
	// {
	// 	if (SpatialGrid[i].CellCoordinates == CellID)
	// 		return i;
	// }
	//
	// return -1;
}

FVector USpatialMemoryComponent::FromCellToWorldCoord(FIntVector CellId)
{
	return (FVector(CellId.X + 0.5f, CellId.Y + 0.5f, CellId.Z + 0.5f)) * FVector(CellSize) - WorldPosOffset;
}

FIntVector USpatialMemoryComponent::GetCellSize()
{
	return CellSize;
}

FSpatialMemoryData USpatialMemoryComponent::GetSpatialMemoryData()
{
	FSpatialMemoryData pingScoreData;
	pingScoreData.SpatialGridCells = SpatialGrid;
	pingScoreData.SpatialMemoryElement = SpatialItemsMemory;
	
	return pingScoreData;
}
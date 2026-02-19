// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpatialMemoryComponent.h"
#include "JackalRobot.h"


// Sets default values for this component's properties
USpatialMemoryComponent::USpatialMemoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USpatialMemoryComponent::AddSpatialMemoryElement(FSpatialMemoryElement NewElement)
{
	SpatialItemsMemory.Add(NewElement);
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
	NewElement.ElementID = DetectedActor->GetUniqueID();

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

	AddSpatialMemoryElement(NewElement);
}

void USpatialMemoryComponent::MergeSpatialMemoryMaps(TArray<FSpatialMemoryElement> OtherMap)
{
	/**
	 * TD - A COMPLETER - TD
	 */

}

TArray<FSpatialMemoryElement> USpatialMemoryComponent::GetSpatialItemsMemory()
{
	return SpatialItemsMemory;
}

void USpatialMemoryComponent::ExploreCell(FIntVector CellID)
{
	/**
	 * TD - A COMPLETER - TD
	 */

}

FIntVector USpatialMemoryComponent::GetCurrentCell()
{
	/**
	 * TD - A COMPLETER - TD
	 */
	return FIntVector();
}

TArray<FSpatialGridCell> USpatialMemoryComponent::GetSpatialGrid()
{
	return SpatialGrid;
}




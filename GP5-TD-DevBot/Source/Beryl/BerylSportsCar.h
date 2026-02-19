// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BerylPawn.h"
#include "BerylSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class BERYL_API ABerylSportsCar : public ABerylPawn
{
	GENERATED_BODY()
	
public:

	ABerylSportsCar();
};

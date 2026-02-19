// Copyright Epic Games, Inc. All Rights Reserved.

#include "BerylWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UBerylWheelRear::UBerylWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}
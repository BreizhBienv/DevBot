// Copyright Epic Games, Inc. All Rights Reserved.

#include "BerylWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UBerylWheelFront::UBerylWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}
// Copyright Epic Games, Inc. All Rights Reserved.

#include "BerylGameMode.h"
#include "BerylPlayerController.h"

ABerylGameMode::ABerylGameMode()
{
	PlayerControllerClass = ABerylPlayerController::StaticClass();
}

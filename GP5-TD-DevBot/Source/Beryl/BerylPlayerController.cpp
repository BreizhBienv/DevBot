// Copyright Epic Games, Inc. All Rights Reserved.


#include "BerylPlayerController.h"
#include "BerylPawn.h"
#include "BerylUI.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void ABerylPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// spawn the UI widget and add it to the viewport
	VehicleUI = CreateWidget<UBerylUI>(this, VehicleUIClass);

	check(VehicleUI);

	VehicleUI->AddToViewport();
}

void ABerylPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		// optionally add the steering wheel context
		if (bUseSteeringWheelControls && SteeringWheelInputMappingContext)
		{
			Subsystem->AddMappingContext(SteeringWheelInputMappingContext, 1);
		}
	}
}

void ABerylPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
	}
}

void ABerylPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	VehiclePawn = CastChecked<ABerylPawn>(InPawn);
}

// Copyright Epic Games, Inc. All Rights Reserved.


#include "ConveyorSystemPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AConveyorSystemPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
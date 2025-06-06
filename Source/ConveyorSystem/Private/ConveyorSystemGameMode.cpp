// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConveyorSystemGameMode.h"
#include "ConveyorSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AConveyorSystemGameMode::AConveyorSystemGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

}

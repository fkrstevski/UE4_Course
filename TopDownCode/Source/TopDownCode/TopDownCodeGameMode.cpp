// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TopDownCodeGameMode.h"
#include "TopDownCodePlayerController.h"
#include "TopDownCodeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATopDownCodeGameMode::ATopDownCodeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownCodePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
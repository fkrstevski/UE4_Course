// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealProjectGameMode.h"
#include "UnrealProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealProjectGameMode::AUnrealProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4LabGameModeBase.h"
#include "UE4LabPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AUE4LabGameModeBase::AUE4LabGameModeBase(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = AUE4LabPlayerController::StaticClass();

	//Gets in PlayerPawnBPClass.Object the HeroCharacterBlueprint class created and configure from the Editor
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/Character/HeroCharacter_BP.HeroCharacter_BP_C'"));

	if (PlayerPawnBPClass.Object != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Object;
	}
}





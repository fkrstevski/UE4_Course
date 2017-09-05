// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller3DGameModeBase.h"
#include "UE4DemoPlayerController.h"
#include "UObject/ConstructorHelpers.h"



ASideScroller3DGameModeBase::ASideScroller3DGameModeBase(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = AUE4DemoPlayerController::StaticClass();

	//Gets in PlayerPawnBPClass.Object the HeroCharacterBlueprint class created and configure from the Editor
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/HeroCharacterBlueprint.HeroCharacterBlueprint_C'"));

	if (PlayerPawnBPClass.Object != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Change default pawn class to HeroCharacterBlueprint"));
		DefaultPawnClass = PlayerPawnBPClass.Object;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4LabPlayerController.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"

/** Inherit method of the AActor class it’s called automatically by the engine when the game starts. */
void AUE4LabPlayerController::BeginPlay()
{
	//Call to the Begin Play method in the base class.
	Super::BeginPlay();

	//We iterate among all the actors in the level through the TActorIterator
	//TActorIterator is a parametrized iterator that allow us to iterate among all actors in the level.
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		//We get the current actor in the loop. As we only have an ACameraActor in the level, the iterator will iterate only once.
		ACameraActor* MainCamera = *It;
		if (MainCamera->GetName().Compare("MyCameraActor") == 0)
		{
			this->SetViewTargetWithBlend(MainCamera);
		}
	}
}



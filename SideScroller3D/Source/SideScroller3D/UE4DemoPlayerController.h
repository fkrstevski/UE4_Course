// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UE4DemoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API AUE4DemoPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
	
};

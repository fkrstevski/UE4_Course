// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TopDownCodeGameMode.generated.h"

UCLASS(minimalapi)
class ATopDownCodeGameMode : public AGameMode
{
	GENERATED_BODY()

	FDateTime m_matchStartTime, m_matchEndTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameMode", meta = (AllowPrivateAccess = "true"))
	float m_timeUntilGameEnds;  // time in seconds until the game ends


public:
	ATopDownCodeGameMode();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

	UFUNCTION(BlueprintNativeEvent)
	bool ReadyToEndMatch();

};




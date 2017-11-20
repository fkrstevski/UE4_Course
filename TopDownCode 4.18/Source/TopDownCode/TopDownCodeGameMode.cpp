// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TopDownCodeGameMode.h"
#include "TopDownCodePlayerController.h"
#include "TopDownCodeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TopDownCode.h"

ATopDownCodeGameMode::ATopDownCodeGameMode()
	: Super()
	, m_timeUntilGameEnds(100.f)
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

void ATopDownCodeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	m_timeUntilGameEnds -= DeltaSeconds;
}
void ATopDownCodeGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTopDownCode, Log, TEXT("ATopDownCodeGameMode Begin Play"));
}

void ATopDownCodeGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	UE_LOG(LogTopDownCode, Log, TEXT("ATopDownCodeGameMode HandleMatchHasStarted"));
	m_matchStartTime = FDateTime::UtcNow();
	UE_LOG(LogTopDownCode, Log, TEXT("Date: %d-%d-%d Time: %d:%d:%d:%d"),
		m_matchStartTime.GetMonth(), m_matchStartTime.GetDay(), m_matchStartTime.GetYear(),
		m_matchStartTime.GetHour(), m_matchStartTime.GetMinute(), m_matchStartTime.GetSecond(), m_matchStartTime.GetMillisecond());

}

void ATopDownCodeGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	UE_LOG(LogTopDownCode, Log, TEXT("ATopDownCodeGameMode HandleMatchHasEnded"));
	m_matchEndTime = FDateTime::UtcNow();
	UE_LOG(LogTopDownCode, Log, TEXT("Match Ended at: %d-%d-%d Time: %d:%d:%d:%d"),
		m_matchEndTime.GetMonth(), m_matchEndTime.GetDay(), m_matchEndTime.GetYear(),
		m_matchEndTime.GetHour(), m_matchEndTime.GetMinute(), m_matchEndTime.GetSecond(), m_matchEndTime.GetMillisecond());

	// for each player controller, show the powerup stats
	for (auto playerIt = GetWorld()->GetPlayerControllerIterator(); playerIt; ++playerIt)
	{
		UE_LOG(LogTopDownCode, Log, TEXT("Player %s:"), *(playerIt->Get()->GetName()));
		ATopDownCodePlayerController* pPlayerController = Cast<ATopDownCodePlayerController>(playerIt->Get());
		check(pPlayerController != nullptr);
		if (pPlayerController)
			pPlayerController->ShowPowerupStats();

	}
}

bool ATopDownCodeGameMode::ReadyToEndMatch_Implementation()
{
	if (m_timeUntilGameEnds < 0)
	{
		return true;
	}
	return false;
}

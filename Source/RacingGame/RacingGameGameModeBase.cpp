// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCar.h"
#include "RacingGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"


ARacingGameGameModeBase::ARacingGameGameModeBase()
{

}

void ARacingGameGameModeBase::BeginPlay()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//PlayerPawn->OnDestroyed.AddDynamic(this, &APlayerCar::StartRespawnTimer);
}

void ARacingGameGameModeBase::Tick(float DeltaSeconds)
{

}


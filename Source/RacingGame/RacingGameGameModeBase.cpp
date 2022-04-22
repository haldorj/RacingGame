// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCar.h"
#include "RacingGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"


ARacingGameGameModeBase::ARacingGameGameModeBase()
{

}

void ARacingGameGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    //Bind our Player died delegate to the Gamemode's PlayerDied function.
    if (!OnPlayerDied.IsBound())
    {
        OnPlayerDied.AddDynamic(this, &ARacingGameGameModeBase::PlayerDied);
    }
}

void ARacingGameGameModeBase::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);
}

void ARacingGameGameModeBase::PlayerDied(APlayerCar* PlayerCar)
{
    //Get a reference to our Character's Player Controller
    AController* CharacterController = PlayerCar->GetController();
    RestartPlayer(CharacterController);
}
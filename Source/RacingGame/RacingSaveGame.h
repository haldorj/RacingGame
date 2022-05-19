// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RacingSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		int32 MaxEnergy;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		int32 Energy;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float Health;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		int Minute;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		int Second;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		int Millisecond;
};

/**
 * 
 */
UCLASS()
class RACINGGAME_API URacingSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	URacingSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStats CharacterStats;
};

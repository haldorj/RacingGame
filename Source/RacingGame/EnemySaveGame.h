// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EnemySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FEnemyStats
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
		float MaxArmour;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float Armour;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FRotator Rotation;
};

/**
 * 
 */
UCLASS()
class RACINGGAME_API UEnemySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UEnemySaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString EnemyName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 EnemyIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FEnemyStats EnemyStats;
};

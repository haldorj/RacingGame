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

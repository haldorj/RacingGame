// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CP5.generated.h"

UCLASS()
class RACINGGAME_API ACP5 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACP5();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Checkpoint")
		class UBoxComponent* CollisionBox;
};
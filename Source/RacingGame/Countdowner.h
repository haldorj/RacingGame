// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Countdowner.generated.h"

UCLASS()
class RACINGGAME_API ACountdowner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountdowner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BluePrintCallable)
		bool IsCountingDown();

	UFUNCTION(BluePrintCallable)
		void SetCountingDown();

	UFUNCTION(BluePrintCallable)
		float GetSeconds();

private:
	bool bCountingDown;
	float SecondsLeft;
};

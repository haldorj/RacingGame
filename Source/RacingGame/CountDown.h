// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCar.h"
#include "CountDown.generated.h"


class UTextRenderComponent;


UCLASS()
class RACINGGAME_API ACountDown : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACountDown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		UStaticMeshComponent* MeshComp = nullptr;

	int32 CountdownTime;
	UTextRenderComponent* CountdownText{ nullptr };

	void UpdateTimerDisplay();

	void AdvanceTimer();

	void CountdownHasFinished();

	FTimerHandle CountdownTimerHandle;

	UPROPERTY(EditAnywhere)
		USoundBase* CountDownSound {
		nullptr
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
		TArray<UStaticMeshComponent*> Lights;
};

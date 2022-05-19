// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "HealthPack.generated.h"

UCLASS()
class RACINGGAME_API AHealthPack : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp")
		class UStaticMeshComponent* HealthPackMesh = nullptr;

	// Collision
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// Timer
	FTimerHandle TimerHandle;

public:
	void Respawn();

	UPROPERTY(EditAnywhere, Category = "Audio")
		USoundBase* Repairing = nullptr;
};

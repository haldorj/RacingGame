// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class RACINGGAME_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletVariables")
		class UStaticMeshComponent* BulletMesh = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "BulletVariables")
		float Speed = 2000.f;

	UPROPERTY(EditAnywhere, Category = "BulletVariables")
		float TimeBeforeDestroy = 5.f;

	UPROPERTY(EditAnywhere, Category = "BulletVariables")
		float TimeLived = 0.f;

	UPROPERTY(EditAnywhere, Category = "BulletVariables")
		UParticleSystem* BulletFireFX = nullptr;

	UPROPERTY(EditAnywhere, Category = "BulletVariables")
		USoundBase* BulletFireSound = nullptr;

	// For interacting with other classes / collision.
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

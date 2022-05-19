// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyHomingProjectile.generated.h"

UCLASS()
class RACINGGAME_API AEnemyHomingProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyHomingProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Collision Component for Missile Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		class UBoxComponent* CollisionComp;

	// Static Mesh Component for Missile Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* MissileMesh;

	// Projectile Movement Component for Missile Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		class UProjectileMovementComponent* ProjectileMovement;

	// Explosion
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
		class UParticleSystem* ExplosionFX;

	// Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		class USoundBase* ExplosionSound = nullptr;




private:
	void Explode();

	UPROPERTY(EditAnywhere, Category = "ForceVariables")
		float ImpulseForce = 1000.f;

	// Declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

UCLASS()
class RACINGGAME_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		// Sphere that will detect the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyVariables")
		class USphereComponent* EnemySensingSphere = nullptr;

	UPROPERTY(VisibleAnywhere)
		UShapeComponent* CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		UStaticMeshComponent* PlayerMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		class UHoverComponent* HoverComponentFL = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		class UHoverComponent* HoverComponentFR = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		class UHoverComponent* HoverComponentHL = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		class UHoverComponent* HoverComponentHR = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
		int32 MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		int32 Energy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float MaxArmour;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float Armour;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void Raycast();
	float Steering();

	//
	//	Player Stats
	//

	bool bForwards;
	bool bNitro;
	float NitroTime;
	float PiValue;
	float YaValue;
	FVector SurfaceImpactNormal;

public:

	//
	// Vehicle variables
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float AngularDamping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float LinearDamping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float ForwardForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float TurnTorque;

	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float TraceLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float HoverForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float HoverLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float InAirGravityForce;

private:
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
};

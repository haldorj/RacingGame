// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCar.generated.h"

UCLASS()
class RACINGGAME_API APlayerCar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere)
		UShapeComponent* CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		UStaticMeshComponent* PlayerMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		UCameraComponent* Camera = nullptr;

	UPROPERTY(EditAnywhere, Category = "PlayerVariables")
		USoundBase* Shooting = nullptr;

	UPROPERTY(EditAnywhere, Category = "PlayerVariables")
		USoundBase* OutOfEnergy = nullptr;

	UPROPERTY(EditAnywhere, Category = "PlayerVariables")
		USoundBase* Reloading = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		class UHoverComponent* HoverComponentFL = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		class UHoverComponent* HoverComponentFR = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		class UHoverComponent* HoverComponentHL = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
		class UHoverComponent* HoverComponentHR = nullptr;

	// For spawning Bullets:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<class AHomingProjectile> ActorToSpawn;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveCameraY(float Value);
	void MoveCameraX(float Value);

	// Functions
	void Shoot();
	void Reload();
<<<<<<< HEAD
	void Nitro();
=======
	UStaticMeshComponent* Target();
>>>>>>> Weapon_Missile
	void Raycast();

	// For interacting with other classes / collision.
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	//
	//	Player Stats
	//
	bool bForwards;
	bool bNitro;
	FVector SurfaceImpactNormal;
	float NitroTime;
	float PiValue;
	float YaValue;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float AngularDamping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float LinearDamping;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float ForwardForce;

	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = "PlayerStats")
		float TraceLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		int32 Coins; // int32 = cross platform

		// Function for switching level
	void SwitchLevel(FName LevelName);

};

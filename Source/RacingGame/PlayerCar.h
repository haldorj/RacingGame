// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
		UCameraComponent* RearCamera = nullptr;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVehicle")
		UStaticMeshComponent* Turret = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVehicle")
		USceneComponent* Barrel = nullptr;

	// For spawning Bullets:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<AActor> ActorToSpawn;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveCameraY(float Value);
	void MoveCameraX(float Value);

	// Functions
	void Shoot();
	void Reload();
	void Nitro();
	void ChangeCameraView();

	bool bESCDown;
	void ESCDown();
	void ESCUp();

	void SetTarget();
	class UStaticMeshComponent* Target();
	class UStaticMeshComponent* HomingTarget;
	class UStaticMeshComponent* TargetMesh;

	void Raycast();

	//brings up menu when player wins
	void Winner();
	void WinnerTimeAttack();

	void SaveBestTime();
	void LoadBestTime();

	UFUNCTION(BluePrintCallable)
		void KillPlayer();

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
	float NitroTime;
	float PiValue;
	float YaValue;
	FVector SurfaceImpactNormal;

	
public:
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Target")
		float TargetMaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
		float TargetHealth;

	int CurrentCheckpoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		int CurrentLap = 0;

	// Function for switching level
	void SwitchLevel(FName LevelName);

	UFUNCTION(BluePrintCallable)
		void SaveGame();

	UFUNCTION(BluePrintCallable)
		void LoadGame(bool SetPosition);

	void HealthFunction();
	void HealthMinus();

	//
	// Vehicle variables
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float AngularDamping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		float LinearDamping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
		bool bCameraMode;

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

	// Time taker
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int Millisecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int Second;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int Minute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		float CountDownSeconds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		bool bCountingDown;

	UFUNCTION(BluePrintCallable)
		bool IsCountingDown();

	UFUNCTION(BluePrintCallable)
		void SetCountingDown();

	UFUNCTION(BluePrintCallable)
		float GetCountDownSeconds();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int WinnerMillisecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int WinnerSecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int WinnerMinute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		FString Medal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int BestMillisecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int BestSecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
		int BestMinute;

	//timer:
	FTimerHandle TimerHandle;

protected:
	// For spawning Player:

		FTimerDelegate TimerDel;
protected:
		// Buttons for pause menu

	UFUNCTION(BluePrintCallable)
	void RestartLevel();

	UFUNCTION(BluePrintCallable)
	void TimeAttackLevel();

	UFUNCTION(BluePrintCallable)
	void RacingLevel();


private:
	// VFX for thrusters
	UPROPERTY(EditAnywhere, Category = "PlayerVariables")
		UParticleSystemComponent* ExhaustR = nullptr;

	UPROPERTY(EditAnywhere, Category = "PlayerVariables")
		UParticleSystemComponent* ExhaustL = nullptr;

	// Sounds and effects
	UPROPERTY(EditAnywhere, Category = "PlayerVariables")
		UParticleSystem* ExplosionFX = nullptr;

	UPROPERTY(EditAnywhere, Category = "PlayerVariables")
		USoundBase* ExplosionSound = nullptr;
};

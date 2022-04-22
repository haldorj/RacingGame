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

	UPROPERTY(VisibleAnywhere)
		UShapeComponent* CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		class UHoverComponent* HoverComponentFL = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		class UHoverComponent* HoverComponentFR = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		class UHoverComponent* HoverComponentHL = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
		class UHoverComponent* HoverComponentHR = nullptr;

private:
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

public:
	// Physics variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float Damping;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float Stiffness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPhysics")
		float HoverLength;
};

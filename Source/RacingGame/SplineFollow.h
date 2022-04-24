// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineFollow.generated.h"

UCLASS()
class RACINGGAME_API ASplineFollow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineFollow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Follow();

	UPROPERTY(VisibleAnywhere, Category = "SplineFollow")
		class USplineComponent* SplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineFollow")
		UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineFollow")
		class UArrowComponent* ArrowComponent = nullptr;

	// Ref to enemy class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineFollow")
	class AEnemy* EnemyRef;
};

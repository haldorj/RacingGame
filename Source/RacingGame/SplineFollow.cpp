// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineFollow.h"
#include "Enemy.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASplineFollow::ASplineFollow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	if (SplineComponent)
	{
		SplineComponent->SetupAttachment(RootComponent);;
	}
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASplineFollow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineFollow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Follow();
}

void ASplineFollow::Follow()
{
	// Steering
	FVector EnemyLocation = EnemyRef->GetActorLocation();

	FVector RightVector = SplineComponent->FindRightVectorClosestToWorldLocation(EnemyLocation, ESplineCoordinateSpace::Local);
	FRotator Rotator = UKismetMathLibrary::MakeRotationFromAxes(RightVector,FVector(0), FVector(0));
	FRotator NewRotator = Rotator + FRotator(0, 0, -180);
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Rotator) * 250; // distance of leading object from AI.

	FVector Location = SplineComponent->FindLocationClosestToWorldLocation(EnemyLocation + ForwardVector, ESplineCoordinateSpace::World); // - or + ForwardVector to change direction.
	FVector ArrowLocation = SplineComponent->FindLocationClosestToWorldLocation(EnemyLocation, ESplineCoordinateSpace::World);
	
	EnemyRef->SteeringVector = Location;

	MeshComponent->SetWorldLocation(EnemyRef->SteeringVector);
	ArrowComponent->SetWorldLocationAndRotation(ArrowLocation, NewRotator);

	// Throttle
	FVector ArrowLocationWorld = ArrowComponent->GetComponentLocation();

	FVector Velocity = EnemyRef->GetVelocity();
	float Speed = (Velocity.Size() * 0.036);	// speed in km/h

	FVector XY = FVector(EnemyLocation.X- ArrowLocationWorld.X, EnemyLocation.Y- ArrowLocationWorld.Y, 0.0f);
	float XYSize = XY.Size();

	float MapRangeUnclamped = UKismetMathLibrary::MapRangeUnclamped(Speed*XYSize, 0, 800, 1, 0.2);
	float select = UKismetMathLibrary::SelectFloat(1, MapRangeUnclamped, (Speed*XYSize) < 0);
	EnemyRef->Throttle = UKismetMathLibrary::SelectFloat(1, select, XYSize < 0);

}
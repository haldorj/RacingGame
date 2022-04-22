// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UHoverComponent::UHoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHoverComponent::BeginPlay()
{
	Super::BeginPlay();
	MeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());


}


// Called every frame
void UHoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult OutHit;
	FVector Start = GetComponentLocation();
	FVector End = Start + (-GetUpVector() * TraceLength);

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bTraceComplex = true;

	bool bHit = (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams));
	if (bHit)
	{
		// Hit Information.
		FVector SurfaceImpactNormal = OutHit.ImpactNormal;
		//FVector ImpactNormal = SurfaceImpactNormal;
		FVector HitLocation = OutHit.Location;

		// Get Length between hit and component.
		FVector WorldLocation = (HitLocation - GetComponentLocation());
		float VectorLength = WorldLocation.Size();
		//UE_LOG(LogTemp, Warning, TEXT("VectorSize: %f"), VectorSize);

		// Value from 0 - 1.
		CompressionThis = 1 - (VectorLength / TraceLength);

		// Linear interpolation between two values, functions as suspension.
		HoverForce = (Stiffness * CompressionThis) + (Damping * ((CompressionThis - CompressionLast) / DeltaTime));

		// Creates Force and Location to push the Actor
		FVector Force = (HoverForce * SurfaceImpactNormal);
		FRotator Rotation = MeshComp->GetComponentRotation();
		FVector Location = MeshComp->GetCenterOfMass() + Rotation.RotateVector(GetRelativeLocation());

		// Add force to Component Location
		MeshComp->AddForceAtLocation(Force, Location);

		// Debug Line and Impact Box
		DrawDebugLine(GetWorld(), Start, End, FColor::Green);
		DrawDebugSolidBox(GetWorld(), OutHit.ImpactPoint, FVector(5.f, 5.f, 5.f), FColor::Green);
		CompressionLast = CompressionThis;
	}

	else
	{
		MeshComp->AddForce(FVector(0.f, 0.f, -1) * InAirGravityForce);

		// Displays a red Debug Line if the line doesn't hit a surface
		DrawDebugLine(GetWorld(), Start, End, FColor::Red);
		CompressionLast = 0;
	}
}
#include "Enemy.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "HoverComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HoverForce = 4000.f;
	HoverLength = 100.f;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	CollisionBox->SetupAttachment(MeshComponent);

	// Creating & attaching the Hover Components
	HoverComponentFL = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentFL"));
	HoverComponentFL->SetupAttachment(MeshComponent);

	HoverComponentFR = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentFR"));
	HoverComponentFR->SetupAttachment(MeshComponent);

	HoverComponentHL = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentHL"));
	HoverComponentHL->SetupAttachment(MeshComponent);

	HoverComponentHR = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentHR"));
	HoverComponentHR->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Hover Physics (overriding HoverComponent.cpp)
	HoverComponentFL->HoverForce = HoverForce / 4;
	HoverComponentFL->TraceLength = HoverLength;

	HoverComponentFR->HoverForce = HoverForce / 4;
	HoverComponentFR->TraceLength = HoverLength;

	HoverComponentHL->HoverForce = HoverForce / 4;
	HoverComponentHL->TraceLength = HoverLength;

	HoverComponentHR->HoverForce = HoverForce / 4;
	HoverComponentHR->TraceLength = HoverLength;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


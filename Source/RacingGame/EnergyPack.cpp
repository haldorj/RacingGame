// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyPack.h"

// Sets default values
AEnergyPack::AEnergyPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	EnergyPackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	EnergyPackMesh->SetupAttachment(GetRootComponent());
	Cast<UShapeComponent>(RootComponent)->OnComponentBeginOverlap.AddDynamic(this, &AEnergyPack::OnOverlap);
}

// Called when the game starts or when spawned
void AEnergyPack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnergyPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnergyPack::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCrate.h"

// Sets default values
AWeaponCrate::AWeaponCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	WeaponCrateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	WeaponCrateMesh->SetupAttachment(GetRootComponent());
	Cast<UShapeComponent>(RootComponent)->OnComponentBeginOverlap.AddDynamic(this, &AWeaponCrate::OnOverlap);
}

// Called when the game starts or when spawned
void AWeaponCrate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponCrate::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}


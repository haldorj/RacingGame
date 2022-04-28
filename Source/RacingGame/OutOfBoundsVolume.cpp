// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfBoundsVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Sets default values
AOutOfBoundsVolume::AOutOfBoundsVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//Size Of Box
	CollisionBox->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AOutOfBoundsVolume::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AOutOfBoundsVolume::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AOutOfBoundsVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOutOfBoundsVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOutOfBoundsVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AOutOfBoundsVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}




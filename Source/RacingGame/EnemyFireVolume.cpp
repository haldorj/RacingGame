// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFireVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemyFireVolume::AEnemyFireVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

}

// Called when the game starts or when spawned
void AEnemyFireVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyFireVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

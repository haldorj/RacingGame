// Fill out your copyright notice in the Description page of Project Settings.


#include "CP4.h"
#include "Components/BoxComponent.h"

// Sets default values
ACP4::ACP4()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//Size Of Box
	CollisionBox->SetBoxExtent(FVector(50.f, 270.f, 270.f));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;
}

// Called when the game starts or when spawned
void ACP4::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACP4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

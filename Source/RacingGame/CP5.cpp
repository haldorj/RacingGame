// Fill out your copyright notice in the Description page of Project Settings.


#include "CP5.h"
#include "Components/BoxComponent.h"

// Sets default values
ACP5::ACP5()
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
void ACP5::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACP5::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Countdowner.h"

// Sets default values
ACountdowner::ACountdowner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCountingDown = false;
	SecondsLeft = 3.f;
}

// Called when the game starts or when spawned
void ACountdowner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACountdowner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bCountingDown) { SecondsLeft -= DeltaTime; }

	if (SecondsLeft >= 0.f) 
	{ 
		SecondsLeft = 0.f;
		bCountingDown = false;
	}
}

bool ACountdowner::IsCountingDown()
{
	return bCountingDown;
}

void ACountdowner::SetCountingDown()
{
	bCountingDown = true;
}

float ACountdowner::GetSeconds()
{
	return SecondsLeft;
}
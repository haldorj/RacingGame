// Fill out your copyright notice in the Description page of Project Settings.


#include "AdjustTorqueVolume.h"
#include "SplineFollow.h"
#include "Enemy.h"

#include "Components/BoxComponent.h"

// Sets default values
AAdjustTorqueVolume::AAdjustTorqueVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAdjustTorqueVolume::OnOverlap);

}

// Called when the game starts or when spawned
void AAdjustTorqueVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAdjustTorqueVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAdjustTorqueVolume::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor->IsA(AEnemy::StaticClass())))
	{
		SplineFollow->ObjectDistance = 3200;
		SplineFollow->ThrottleMultiplier = 1;

		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Overlap")));
	}
}


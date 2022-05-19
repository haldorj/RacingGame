// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineFollowAlterationReset.h"
#include "SplineFollow.h"
#include "Enemy.h"

#include "Components/BoxComponent.h"


// Sets default values
ASplineFollowAlterationReset::ASplineFollowAlterationReset()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASplineFollowAlterationReset::OnOverlap);

}

// Called when the game starts or when spawned
void ASplineFollowAlterationReset::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASplineFollowAlterationReset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineFollowAlterationReset::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if ((OtherActor->IsA(AEnemy::StaticClass())))
	{
		SplineFollow->ObjectDistance = 2200;
		SplineFollow->ThrottleMultiplier = 0.6;

		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Overlap")));
	}
}


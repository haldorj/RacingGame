// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmourPack.h"

// Sets default values
AArmourPack::AArmourPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	ArmourPackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	ArmourPackMesh->SetupAttachment(GetRootComponent());
	Cast<UShapeComponent>(RootComponent)->OnComponentBeginOverlap.AddDynamic(this, &AArmourPack::OnOverlap);
}

// Called when the game starts or when spawned
void AArmourPack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArmourPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArmourPack::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}


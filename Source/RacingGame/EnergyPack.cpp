// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyPack.h"
#include "PlayerCar.h"
#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

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
	if (OtherActor->IsA(APlayerCar::StaticClass()))
	{
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		
		APlayerCar* PlayerCar = Cast<APlayerCar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		PlayerCar->Energy++;

		UWorld* NewWorld = GetWorld();
		UGameplayStatics::PlaySound2D(NewWorld, Reloading, 1.f, 1.f, 0.f, 0);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnergyPack::Respawn, 5.f, false);
		if (PlayerCar->Energy > PlayerCar->MaxEnergy)
		{
			PlayerCar->Energy = PlayerCar->MaxEnergy;
		}
	}
	if (OtherActor->IsA(AEnemy::StaticClass()))
	{
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnergyPack::Respawn, 5.f, false);
	}
}

void AEnergyPack::Respawn()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPack.h"
#include "PlayerCar.h"
#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AHealthPack::AHealthPack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	HealthPackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	HealthPackMesh->SetupAttachment(GetRootComponent());
	Cast<UShapeComponent>(RootComponent)->OnComponentBeginOverlap.AddDynamic(this, &AHealthPack::OnOverlap);
}

// Called when the game starts or when spawned
void AHealthPack::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPack::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCar::StaticClass()))
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		APlayerCar* PlayerCar = Cast<APlayerCar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		PlayerCar->Health+= 35;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AHealthPack::Respawn, 5.f, false);
		if (PlayerCar->Health > PlayerCar->MaxHealth)
		{
			PlayerCar->Health = PlayerCar->MaxHealth;
		}
	}

	if (OtherActor->IsA(AEnemy::StaticClass()))
	{
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);

		UWorld* NewWorld = GetWorld();
		UGameplayStatics::PlaySound2D(NewWorld, Repairing, 1.f, 1.f, 0.f, 0);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AHealthPack::Respawn, 5.f, false);
	}
}

void AHealthPack::Respawn()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

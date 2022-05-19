// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "PlayerCar.h"
#include "HoverComponent.h"
#include "Bullet.h"
#include "HealthPack.h"
#include "HomingProjectile.h"
#include "HoverComponent.h"
#include "ArmourPack.h"
#include "EnergyPack.h"
#include "WeaponCrate.h"
#include "EnemySaveGame.h"
#include "MainPlayerController.h"
#include "CheckPoint.h"
#include "OutOfBoundsVolume.h"
#include "RacingGameGameModeBase.h"
#include "CP1.h"
#include "CP2.h"
#include "CP3.h"
#include "SplineFollowAlteration.h"
#include "SplineFollowAlterationReset.h"
#include "NitroVolumeNPC.h"
#include "EnemyFireVolume.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(PlayerMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	CollisionBox->SetupAttachment(RootComponent);

	//CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	//CombatSphere->SetGenerateOverlapEvents(true);
	//CombatSphere->SetSphereRadius(2000.f);
	//CombatSphere->SetupAttachment(RootComponent);
	//
	//CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	//CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	// Creating & attaching the Hover Components
	HoverComponentFL = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentFL"));
	HoverComponentFL->SetupAttachment(PlayerMesh);

	HoverComponentFR = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentFR"));
	HoverComponentFR->SetupAttachment(PlayerMesh);

	HoverComponentHL = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentHL"));
	HoverComponentHL->SetupAttachment(PlayerMesh);

	HoverComponentHR = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentHR"));
	HoverComponentHR->SetupAttachment(PlayerMesh);

	// Defining Physics-related Values
	AngularDamping = 5.f;
	LinearDamping = 2.f;
	ForwardForce = 2000.f;
	TurnTorque = 10000.f;
	HoverForce = 350.f;
	HoverLength = 100.f;
	TraceLength = 130;

	Energy = 0;
	Health = 100.f;
	Armour = 0.f;
	MaxEnergy = 3;
	MaxHealth = 100.f;
	MaxArmour = 35.f;

	// Defining Car Values
	bForwards = true;
	bNitro = false;
	bStun = false;

	Exhaust = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Exhaust"));
	Exhaust->AttachTo(PlayerMesh);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Exhaust->Deactivate();
	
	PlayerMesh->SetAngularDamping(AngularDamping);
	PlayerMesh->SetLinearDamping(LinearDamping);

	// Hover Physics (overriding HoverComponent.cpp)
	HoverComponentFL->HoverForce = HoverForce;
	HoverComponentFL->TraceLength = HoverLength;
	HoverComponentFL->InAirGravityForce = InAirGravityForce;

	HoverComponentFR->HoverForce = HoverForce;
	HoverComponentFR->TraceLength = HoverLength;
	HoverComponentFR->InAirGravityForce = InAirGravityForce;

	HoverComponentHL->HoverForce = HoverForce;
	HoverComponentHL->TraceLength = HoverLength;
	HoverComponentHL->InAirGravityForce = InAirGravityForce;

	HoverComponentHR->HoverForce = HoverForce;
	HoverComponentHR->TraceLength = HoverLength;
	HoverComponentHR->InAirGravityForce = InAirGravityForce;

	PlayerMesh->SetAngularDamping(AngularDamping);

	TimerDel.BindUFunction(this, FName("EnemyLoadGame"), true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APlayerCar* Player = Cast<APlayerCar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (Player != NULL && Player->IsCountingDown() == false)
	{
		MoveForward(Throttle);
		MoveRight(Steering());
		Raycast();
		HealthFunction();
	}

	if (bStun) {
		if (StunTime > 0) {
			StunTime -= DeltaTime;
		}
		else {
			bStun = false;
			StunTime = 0;
			ForwardForce *= 5;
		}
	}

	// Bring up defeat screen
	if (CurrentLap == 4)
	{
		EnemyWinner();
	}

	// If the Nitro is on, increase the forward force, else, reduce it back 
	if (bNitro) {
		if (NitroTime > 0) {
			NitroTime -= DeltaTime;
			Exhaust->Activate();
		}
		else {
			bNitro = false;
			NitroTime = 0;
			ForwardForce /= 1.3f;
			Exhaust->Deactivate();
		}
	}
}

// Called to bind functionality to input
//void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void AEnemy::Raycast()
{
	FHitResult OutHit;
	FVector Start;
	if (bForwards) { Start = PlayerMesh->GetComponentLocation() + PlayerMesh->GetForwardVector() * 60; }
	else { Start = PlayerMesh->GetComponentLocation() + PlayerMesh->GetForwardVector() * -60; }
	FVector End = Start + (PlayerMesh->GetUpVector() * (-TraceLength));

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams));
	if (bHit)
	{
		// Hit Information.
		SurfaceImpactNormal = OutHit.ImpactNormal;

		// Simulate Drag in all axis
		FVector Drag = GetVelocity() * -LinearDamping;
		PlayerMesh->AddForceAtLocation(Drag * PlayerMesh->GetMass(), PlayerMesh->GetCenterOfMass());

		//DrawDebugSolidBox(GetWorld(), OutHit.ImpactPoint, FVector(5, 5, 5), FColor::Cyan, false, -1);
	}

	else {
		SurfaceImpactNormal = FVector(0.f, 0.f, 1.f);

		FVector Drag = GetVelocity() * -LinearDamping;
		Drag = FVector(Drag.X, Drag.Y, 0.f);
		PlayerMesh->AddForceAtLocation(Drag * PlayerMesh->GetMass(), PlayerMesh->GetCenterOfMass());
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, -1, 0, 1);
}

float AEnemy::Steering()
{
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SteeringVector);
	FRotator NormalizedAB = UKismetMathLibrary::NormalizedDeltaRotator(Rotation, GetActorRotation());
	SteeringValue = UKismetMathLibrary::MapRangeClamped(NormalizedAB.Yaw, -90, 90, -1, 1);

	return SteeringValue;
}

void AEnemy::Stun()
{
	if (bStun == false) {
		bStun = true;

		StunTime = 2.f;
		ForwardForce *= 0.2f;
	}
}

void AEnemy::Nitro() {
	if (bNitro == false && Energy >= 2) {
		bNitro = true;
		Energy -= 2;

		NitroTime = 3.f;
		ForwardForce *= 1.3f;
	}
}

void AEnemy::Shoot()
{
	FVector Location = GetActorLocation() + FVector(0, 0, 200);
	FRotator Rotation = GetActorRotation();

	UWorld* World = GetWorld();
	if (World)
	{
		if (Energy >= 1)
		{
			World->SpawnActor<AActor>(ActorToSpawn, Location, Rotation);
			Energy--;
		}
	}
}

void AEnemy::EnemyWinner()
{
	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (MainPlayerController)
	{
		MainPlayerController->DisplayLoseScreen();
	}
}

void AEnemy::MoveForward(float Value)
{
	FVector Projection = UKismetMathLibrary::ProjectVectorOnToPlane(GetActorForwardVector(), SurfaceImpactNormal);
	FVector Force = (ForwardForce * Projection * PlayerMesh->GetMass());

	FVector Center = PlayerMesh->GetCenterOfMass();
	FVector Varience(0.f, 0.f, 1.f);

	if (HoverComponentFL->bHit == false && HoverComponentFR->bHit == false && HoverComponentHL->bHit == false && HoverComponentHR->bHit == false)
	{
		Varience = FVector(0.f, 0.f, -0.5f);
	}
	else
	{
		Varience = FVector(0.f, 0.f, 1.f);
	}

	PlayerMesh->AddForceAtLocation((Force * Value), Center + Varience);

	if (Value < 0) { bForwards = false; }
	else if (Value > 0) { bForwards = true; }
}

void AEnemy::MoveRight(float Value)
{
	float Torque = TurnTorque * PlayerMesh->GetMass();

	// Backwards steering functionality
	float Select;
	if (bForwards) { Select = 1; }
	else if (!bForwards) { Select = -1; }

	FRotator Rotation = GetActorRotation();
	FVector TorqueVector = Rotation.RotateVector(FVector(0.f, 0.f, Select * Torque));

	PlayerMesh->AddTorqueInRadians(TorqueVector * Value);

}

void AEnemy::KillPlayer()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	//ParticleFX:
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetTransform(), true);

	//SoundFX
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 3.f, false);
}

void AEnemy::HealthFunction()
{
	if (Health <= 0)
	{
		Health = 0.01;
		KillPlayer();
	}
}

void AEnemy::EnemySaveGame()
{
	UEnemySaveGame* SaveGameInstance = Cast<UEnemySaveGame>(UGameplayStatics::CreateSaveGameObject(UEnemySaveGame::StaticClass()));

	SaveGameInstance->EnemyStats.Health = Health;
	SaveGameInstance->EnemyStats.MaxHealth = MaxHealth;
	SaveGameInstance->EnemyStats.Energy = Energy;
	SaveGameInstance->EnemyStats.MaxEnergy = MaxEnergy;
	SaveGameInstance->EnemyStats.Armour = Armour;
	SaveGameInstance->EnemyStats.MaxArmour = MaxArmour;

	SaveGameInstance->EnemyStats.Location = GetActorLocation();
	SaveGameInstance->EnemyStats.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->EnemyName, SaveGameInstance->EnemyIndex);
}

void AEnemy::EnemyLoadGame(bool SetPosition)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	UEnemySaveGame* LoadGameInstance = Cast<UEnemySaveGame>(UGameplayStatics::CreateSaveGameObject(UEnemySaveGame::StaticClass()));

	LoadGameInstance = Cast<UEnemySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->EnemyName, LoadGameInstance->EnemyIndex));

	Health = LoadGameInstance->EnemyStats.Health;
	MaxHealth = LoadGameInstance->EnemyStats.MaxHealth;
	Energy = LoadGameInstance->EnemyStats.Energy;
	MaxEnergy = LoadGameInstance->EnemyStats.MaxEnergy;
	Armour = LoadGameInstance->EnemyStats.Armour;
	MaxArmour = LoadGameInstance->EnemyStats.MaxArmour;

	if (SetPosition)
	{
		SetActorLocation((LoadGameInstance->EnemyStats.Location) + (FVector(0, 0, 10.f)));
		SetActorRotation(LoadGameInstance->EnemyStats.Rotation);
	}
}


void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AHealthPack::StaticClass()))
	{
		Health += 30;

		if (Health > MaxHealth)
		{
			Health = MaxHealth;
		}

	}

	if (OtherActor->IsA(AEnergyPack::StaticClass()))
	{
		Energy++;

		if (Energy > MaxEnergy)
		{
			Energy = MaxEnergy;
		}
	}

	if (OtherActor->IsA(AHomingProjectile::StaticClass()))
	{
		Health -= 35.f;
		Stun();
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Health %f "), Health));
	}

	if (OtherActor->IsA(ACheckPoint::StaticClass()))
	{
		EnemySaveGame();
	}

	if (OtherActor->IsA(AOutOfBoundsVolume::StaticClass()))
	{
		EnemyLoadGame(true);
	}

	if (OtherActor->IsA(ACP1::StaticClass()) && CurrentCheckpoint == 0)
	{
		CurrentCheckpoint += 1;
		CurrentLap += 1;
	}

	if (OtherActor->IsA(ACP2::StaticClass()) && CurrentCheckpoint == 1)
	{
		CurrentCheckpoint += 1;
	}

	if (OtherActor->IsA(ACP3::StaticClass()) && CurrentCheckpoint == 2)
	{
		CurrentCheckpoint = 0;
	}

	if (OtherActor->IsA(ASplineFollowAlteration::StaticClass()))
	{
		TurnTorque = 10000000;
	}

	if (OtherActor->IsA(ASplineFollowAlterationReset::StaticClass()))
	{
		TurnTorque = 1000000;
	}

	if (OtherActor->IsA(ANitroVolumeNPC::StaticClass()))
	{
		Nitro();
	}

	if (OtherActor->IsA(AEnemyFireVolume::StaticClass()))
	{
		Shoot();
	}
}

//void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
//	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor)
//	{
//		APlayerCar* PlayerCar = Cast<APlayerCar>(OtherActor);
//		{
//			if (PlayerCar)
//			{
//				PlayerCar->SetCombatTarget(this);
//				PlayerCar->TargetMaxHealth = MaxHealth;
//				PlayerCar->TargetHealth = Health;
//				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Overlap ")));
//			}
//		}
//	}
//}
//
//void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (OtherActor)
//	{
//		APlayerCar* PlayerCar = Cast<APlayerCar>(OtherActor);
//		{
//			if (PlayerCar)
//			{
//				PlayerCar->SetCombatTarget(nullptr);
//				PlayerCar->TargetMaxHealth = MaxHealth;
//
//			}
//		}
//	}
//}

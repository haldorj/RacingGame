// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCar.h"
#include "HoverComponent.h"
#include "DrawDebugHelpers.h"
#include "Bullet.h"
#include "HealthPack.h"
#include "HomingProjectile.h"
#include "HoverComponent.h"
#include "ArmourPack.h"
#include "EnergyPack.h"
#include "WeaponCrate.h"
#include <algorithm>
#include "GameFramework/PlayerInput.h"
#include "Camera/CameraActor.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APlayerCar::APlayerCar()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Defining Player Values
	Energy = 3;
	Health = 20.f;
	Armour = 0.f;
	MaxEnergy = 3;
	MaxHealth = 100.f;
	MaxArmour = 35.f;

	// Defining Camera Values
	PiValue = -15.f;
	YaValue = 0.f;

	// Defining Car Values
	bForwards = true;
	bNitro = false;

	// Defining Physics-related Values
	AngularDamping = 5.f;
	LinearDamping = 1.f;
	ForwardForce = 4000.f;
	TurnTorque = 4000.f;
	TraceLength = 120.f;
	Damping = 50.f;
	Stiffness = 500.f;
	HoverLength = 100.f;

	// Creating & rooting Player Mesh
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	SetRootComponent(PlayerMesh);

	// Creating & attaching Collision Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCar::OnOverlap);
	CollisionBox->SetupAttachment(PlayerMesh);

	// Creating & rotating Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(0.f, -15.f, 0.f));
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 7.f;
	SpringArm->SetupAttachment(PlayerMesh);

	// Creating & attaching Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(0.f, 15.f, 0.f));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Creating & attaching the Hover Components
	HoverComponentFL = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentFL"));
	HoverComponentFL->SetupAttachment(PlayerMesh);

	HoverComponentFR = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentFR"));
	HoverComponentFR->SetupAttachment(PlayerMesh);

	HoverComponentHL = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentHL"));
	HoverComponentHL->SetupAttachment(PlayerMesh);

	HoverComponentHR = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentHR"));
	HoverComponentHR->SetupAttachment(PlayerMesh);
}

// Called when the game starts or when spawned
void APlayerCar::BeginPlay()
{
	Super::BeginPlay();

	//// Restriction physics
	PlayerMesh->SetAngularDamping(AngularDamping);

	// Hover Physics (overriding HoverComponent.cpp)
	HoverComponentFL->Damping = Damping * PlayerMesh->GetMass();
	HoverComponentFL->Stiffness = Stiffness * PlayerMesh->GetMass();
	HoverComponentFL->TraceLength = HoverLength;

	HoverComponentFR->Damping = Damping * PlayerMesh->GetMass();
	HoverComponentFR->Stiffness = Stiffness * PlayerMesh->GetMass();
	HoverComponentFR->TraceLength = HoverLength;

	HoverComponentHL->Damping = Damping * PlayerMesh->GetMass();
	HoverComponentHL->Stiffness = Stiffness * PlayerMesh->GetMass();
	HoverComponentHL->TraceLength = HoverLength;

	HoverComponentHR->Damping = Damping * PlayerMesh->GetMass();
	HoverComponentHR->Stiffness = Stiffness * PlayerMesh->GetMass();
	HoverComponentHR->TraceLength = HoverLength;
}

// Called every frame
void APlayerCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// If the Nitro is on, increase the forward force, else, reduce it back 
	if (bNitro) {
		if (NitroTime > 0) {
			NitroTime -= DeltaTime;
		}
		else {
			bNitro = false;
			NitroTime = 0;
			ForwardForce /= 1.3f;
		}
	}

	// Checking Surface Normal
	Raycast();

	//// Axis-based LinearDamping Testing
	//FVector LinearVelocity = GetVelocity();
	//FVector linearDamping = FVector(LinearDamping, LinearDamping, 0.1f);
	//float DeltaSeconds = DeltaTime;

	//FVector LinearDampingTimesDeltaSeconds = linearDamping * DeltaSeconds;
	//FVector LinearVelocityMultiplier = FVector(
	//	std::max(0.0f, 1.0f - LinearDampingTimesDeltaSeconds.X),
	//	std::max(0.0f, 1.0f - LinearDampingTimesDeltaSeconds.Y),
	//	std::max(0.0f, 1.0f - LinearDampingTimesDeltaSeconds.Z));
	//LinearVelocity *= LinearVelocityMultiplier;

	// Anti-Gravity Movement Prototype
	float Gravity;
	Gravity = 981.f;
	PlayerMesh->AddForceAtLocation(-SurfaceImpactNormal * Gravity * PlayerMesh->GetMass(), PlayerMesh->GetCenterOfMass());
}

// Called to bind functionality to input
void APlayerCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent != nullptr);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCar::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCar::MoveRight);
	InputComponent->BindAxis(TEXT("MoveCameraY"), this, &APlayerCar::MoveCameraY);
	InputComponent->BindAxis(TEXT("MoveCameraX"), this, &APlayerCar::MoveCameraX);

	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &APlayerCar::Shoot);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &APlayerCar::Reload);
	PlayerInputComponent->BindAction("Nitro", EInputEvent::IE_Pressed, this, &APlayerCar::Nitro);
}

void APlayerCar::MoveForward(float Value)
{
	FVector Projection = UKismetMathLibrary::ProjectVectorOnToPlane(GetActorForwardVector(), SurfaceImpactNormal);
	FVector Force = (ForwardForce * Projection * PlayerMesh->GetMass());

	FVector Center = PlayerMesh->GetCenterOfMass();
	FVector Varience(0.f, 0.f, 1.5f);
	PlayerMesh->AddForceAtLocation((Force * Value), Center + Varience);

	if (Value < 0) { bForwards = false; }
	else if (Value > 0) { bForwards = true; }
}

void APlayerCar::MoveRight(float Value)
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

void APlayerCar::MoveCameraY(float Value)
{
	PiValue += Value;

	SpringArm->SetRelativeRotation(FRotator(PiValue, YaValue, 0.f));
}

void APlayerCar::MoveCameraX(float Value) 
{
	YaValue += Value;

	SpringArm->SetRelativeRotation(FRotator(PiValue, YaValue, 0.f));
}

void APlayerCar::Shoot()
{
	if (ActorToSpawn != NULL) 
	{
		if (Energy > 0)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				Energy--;
				FVector Location = GetActorLocation();
				FRotator Rotation = GetActorRotation();

				if (ActorToSpawn->GetName() == "Bullet_BP_C") {
					World->SpawnActor<AActor>(ActorToSpawn, Location + Rotation.RotateVector(FVector(350.f, 0.f, 80.f)), Rotation);
				}
				else {
					AHomingProjectile* HomingProjectile = World->SpawnActor<AHomingProjectile>(ActorToSpawn, Location + Rotation.RotateVector(FVector(350.f, 0.f, 80.f)), GetActorRotation());
					if (HomingProjectile) {
						HomingProjectile->ProjectileMovement->HomingTargetComponent = Target();
					}
				}

				UGameplayStatics::PlaySound2D(World, Shooting, 1.f, 1.f, 0.f, 0);
			}
		}

		else if (Energy <= 0)
		{
			Energy = 0;
			UWorld* World = GetWorld();
			if (World)
			{
				UGameplayStatics::PlaySound2D(World, OutOfEnergy, 1.f, 1.f, 0.f, 0);
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Shooting"));
	}
}

void APlayerCar::Reload() {
	Energy = MaxEnergy;
	UWorld* NewWorld = GetWorld();
	UGameplayStatics::PlaySound2D(NewWorld, Reloading, 1.f, 1.f, 0.f, 0);
}

UStaticMeshComponent* APlayerCar::Target()
{
	FHitResult OutHit;
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * 5000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray; // object types to trace
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));

	TArray< AActor* > ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool Hit = (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, objectTypesArray, true, ActorsToIgnore,
		EDrawDebugTrace::Type::ForDuration, OutHit, true));

	UStaticMeshComponent* HomingTarget = Cast<UStaticMeshComponent>(OutHit.GetComponent());
	return HomingTarget;

	if (Hit)
	{
		if (HomingTarget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Hit")));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Not A PhysicsBody ")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("No Hit ")));
	}
}

void APlayerCar::Nitro() {
	if (bNitro == false && Energy >= 2) {
		bNitro = true;
		Energy -= 2;

		NitroTime = 3.f;
		ForwardForce *= 1.3f;
	}
}

void APlayerCar::Raycast()
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

			DrawDebugSolidBox(GetWorld(), OutHit.ImpactPoint, FVector(5, 5, 5), FColor::Cyan, false, -1);
		}

		else {
			SurfaceImpactNormal = FVector(0.f, 0.f, 1.f);
		}

		DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, -1, 0, 1);
}

void APlayerCar::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AHealthPack::StaticClass()))
	{
		Health += 30;

		if (Health > MaxHealth)
		{
			Health = MaxHealth;
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Player Picked Up Health %f "), Health));
		UE_LOG(LogTemp, Warning, TEXT("Player Picked Up Health %f "), Health);
		OtherActor->Destroy();
	}

	if (OtherActor->IsA(AArmourPack::StaticClass()))
	{
		Armour += 20;

		if (Armour > MaxArmour)
		{
			Armour = MaxArmour;
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Player Picked Up Armour %f "), Armour));
		UE_LOG(LogTemp, Warning, TEXT("Player Picked Up Armour %f "), Armour);
		OtherActor->Destroy();
	}

	if (OtherActor->IsA(AEnergyPack::StaticClass()))
	{
		Energy++;

		if (Energy > MaxEnergy)
		{
			Energy = MaxEnergy;
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Player Picked Up Energy %d "), Energy));
		UE_LOG(LogTemp, Warning, TEXT("Player Picked Up Energy %d "), Energy);
		OtherActor->Destroy();
	}

	if (OtherActor->IsA(AWeaponCrate::StaticClass()))
	{
		ActorToSpawn = Cast<AWeaponCrate>(OtherActor)->Weapon;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Player Picked Up Weapon Crate Containing a %s "), *ActorToSpawn->GetName()));
		UE_LOG(LogTemp, Display, TEXT("Player Picked Up Weapon Crate Containing a %s "), *ActorToSpawn->GetName());
		OtherActor->Destroy();
	}
}

void APlayerCar::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();

		FName CurrentLevelName(*CurrentLevel);
		if (CurrentLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(World, LevelName);
		}

	}
}

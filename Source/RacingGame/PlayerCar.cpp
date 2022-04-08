// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCar.h"
#include "HoverComponent.h"
#include "DrawDebugHelpers.h"
#include "Bullet.h"
#include "HoverComponent.h"
#include "HealthPack.h"
#include "ArmourPack.h"
#include "EnergyPack.h"
#include "WeaponCrate.h"
#include "GameFramework/PlayerInput.h"
#include "Camera/CameraActor.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APlayerCar::APlayerCar()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Defining Variables
	Energy = 3;
	MaxEnergy = 3;
	Health = 20.f;
	MaxHealth = 100.f;
	Armour = 0.f;
	MaxArmour = 35.f;
	Coins = 0;
	PiValue = 0.f;
	YaValue = -10.f;
	bForwards = true;
	bNitro = false;

	// Defining Force Variables
	AngularDamping = 5.0f;
	LinearDamping = 1.0f;
	ForwardForce = 4000.0f;
	TraceLength = 60.f;

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
	SpringArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 7.f;
	SpringArm->SetupAttachment(PlayerMesh);

	// Creating & attaching Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
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

	// Restriction physics
	PlayerMesh->SetAngularDamping(AngularDamping);
	PlayerMesh->SetLinearDamping(LinearDamping);
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

	//// Anti-Gravity Movement Prototype
	//float Gravity;
	//Gravity = 981.f;
	//PlayerMesh->AddForceAtLocation(-SurfaceImpactNormal * Gravity * PlayerMesh->GetMass(), PlayerMesh->GetCenterOfMass());
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
	else { bForwards = true; }
}

void APlayerCar::MoveRight(float Value)
{
	float Torque = 30000.f * PlayerMesh->GetMass();

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
					World->SpawnActor<AActor>(ActorToSpawn, Location + Rotation.RotateVector(FVector(160.f, 0.f, 85.f)), Rotation);
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
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Player Picked Up Energy %f "), Energy));
		UE_LOG(LogTemp, Warning, TEXT("Player Picked Up Energy %f "), Energy);
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCar.h"
#include "HoverComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Bullet.h"
//#include "Coin.h"
//#include "HealthPack.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"


// Sets default values
APlayerCar::APlayerCar()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//SetRootComponent(Root);

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	SetRootComponent(PlayerMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCar::OnOverlap);
	CollisionBox->SetupAttachment(PlayerMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 7.f;

	SpringArm->SetupAttachment(PlayerMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Ammo = 6;
	MaxAmmo = 6;
	Health = 25.f;
	MaxHealth = 100.f;
	Coins = 0;
	bForwards = true;

	AngularDamping = 5.0f;
	LinearDamping = 3.0f;

	ForwardForce = 3500.f;

	TraceLength = 100.f;
}

// Called when the game starts or when spawned
void APlayerCar::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Raycast();
	float Velocity;
	Velocity = this->GetVelocity().Size();
	Velocity /= 100;
	Velocity *= 3.6f;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Speed :  %f "), Velocity));
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
}

void APlayerCar::MoveForward(float Value)
{
	FVector Projection = UKismetMathLibrary::ProjectVectorOnToPlane(GetActorForwardVector(), SurfaceImpactNormal);
	FVector Force = (ForwardForce * Projection * PlayerMesh->GetMass());
	PlayerMesh->AddForce(Force * Value);

	PlayerMesh->SetAngularDamping(AngularDamping);
	PlayerMesh->SetLinearDamping(LinearDamping);

	if (Value < 0) { bForwards = false; }
	else if (Value > 0) { bForwards = true; }

}

void APlayerCar::MoveRight(float Value)
{
	float Torque = 3000000.f;

	// Backwards steering functionality
	float Select;
	if (bForwards) { Select = 1; }
	else if (!bForwards) { Select = -1; }
	FVector TorqueVector = FVector(0.f, 0.f, Select * Torque);

	PlayerMesh->AddTorqueInRadians(TorqueVector * Value);
}

void APlayerCar::MoveCameraY(float Value)
{
	SpringArm->AddRelativeRotation(FRotator(Value, 0.f, 0.f));
}

void APlayerCar::MoveCameraX(float Value) 
{
	SpringArm->AddRelativeRotation(FRotator(0.f, Value, 0.f));
}

void APlayerCar::Shoot()
{
	if (Ammo > 0)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			Ammo--;
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Ammo :  %d "), Ammo));
			// "Shotgun"
			// Subject to change

			FVector Location = GetActorLocation();
			FRotator Rotation = GetActorRotation();
			World->SpawnActor<AActor>(ActorToSpawn, Location + FVector(0.f, 0.f, 80.f), Rotation + FRotator(0.f, -4.f, 0.f));
			World->SpawnActor<AActor>(ActorToSpawn, Location + FVector(0.f, 0.f, 80.f), Rotation + FRotator(0.f, -2.f, 0.f));
			World->SpawnActor<AActor>(ActorToSpawn, Location + FVector(0.f, 0.f, 80.f), Rotation + FRotator(0.f, 0.f, 0.f));
			World->SpawnActor<AActor>(ActorToSpawn, Location + FVector(0.f, 0.f, 80.f), Rotation + FRotator(0.f, 2.f, 0.f));
			World->SpawnActor<AActor>(ActorToSpawn, Location + FVector(0.f, 0.f, 80.f), Rotation + FRotator(0.f, 4.f, 0.f));
			UGameplayStatics::PlaySound2D(World, Shooting, 1.f, 1.f, 0.f, 0);
		}
	}

	else if (Ammo <= 0)
	{
		Ammo = 0;
		UWorld* World = GetWorld();
		if (World)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No ammo Reload %d "), Ammo));
			UGameplayStatics::PlaySound2D(World, OutOfAmmo, 1.f, 1.f, 0.f, 0);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Shooting"));
}

void APlayerCar::Reload() {
	Ammo = MaxAmmo;
	UWorld* NewWorld = GetWorld();
	UGameplayStatics::PlaySound2D(NewWorld, Reloading, 1.f, 1.f, 0.f, 0);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Reloaded %d "), Ammo));
}

void APlayerCar::Raycast()
{
	if (bForwards) {
		FHitResult OutHit;
		FVector Start = PlayerMesh->GetComponentLocation() + PlayerMesh->GetForwardVector() * 50;
		FVector End = Start + (PlayerMesh->GetUpVector() * (-TraceLength));

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		//CollisionParams.bTraceComplex = true;

		bool bHit = (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams));
		if (bHit)
		{
			// Hit Information.
			SurfaceImpactNormal = OutHit.ImpactNormal;

			DrawDebugSolidBox(GetWorld(), OutHit.ImpactPoint, FVector(5, 5, 5), FColor::Cyan, false, -1);

			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("X :  %f "), (SurfaceImpactNormal.X)));
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Y:  %f "), (SurfaceImpactNormal.Y)));
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Z:  %f "), (SurfaceImpactNormal.Z)));
		}

		else {
			SurfaceImpactNormal = FVector(0.f, 0.f, 1.f);
		}

		DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, -1, 0, 1);
	}
	else if (!bForwards) {
		FHitResult OutHit;
		FVector Start = PlayerMesh->GetComponentLocation() + PlayerMesh->GetForwardVector() * -50;
		FVector End = Start + (PlayerMesh->GetUpVector() * (-TraceLength));

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		//CollisionParams.bTraceComplex = true;

		bool bHit = (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams));
		if (bHit)
		{
			// Hit Information.
			SurfaceImpactNormal = OutHit.ImpactNormal;

			DrawDebugSolidBox(GetWorld(), OutHit.ImpactPoint, FVector(5, 5, 5), FColor::Cyan, false, -1);

			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("X :  %f "), (SurfaceImpactNormal.X)));
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Y:  %f "), (SurfaceImpactNormal.Y)));
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Z:  %f "), (SurfaceImpactNormal.Z)));
		}

		else {
			SurfaceImpactNormal = FVector(0.f, 0.f, 1.f);
		}

		DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, -1, 0, 1);
	}
}

void APlayerCar::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor->IsA(ACoin::StaticClass()))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::Printf(TEXT("Player Picked Up Coin")));
	//	UE_LOG(LogTemp, Warning, TEXT("Player Picked Up Coin"))
	//		OtherActor->Destroy();
	//	Coins++;
	//}
	//if (OtherActor->IsA(AHealthPack::StaticClass()))
	//{
	//	Health += 20;

	//	if (Health > MaxHealth)
	//	{
	//		Health = MaxHealth;
	//	}
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Player Picked Up Health %f "), Health));
	//	UE_LOG(LogTemp, Warning, TEXT("Player Picked Up Health %f "), Health)
	//		OtherActor->Destroy();
	//}
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

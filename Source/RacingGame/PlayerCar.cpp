// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCar.h"
#include "HoverComponent.h"
#include "Bullet.h"
#include "HealthPack.h"
#include "HomingProjectile.h"
#include "HoverComponent.h"
#include "ArmourPack.h"
#include "EnergyPack.h"
#include "WeaponCrate.h"
#include "RacingSaveGame.h"
#include "MainPlayerController.h"
#include "CheckPoint.h"
#include "OutOfBoundsVolume.h"
#include "RacingGameGameModeBase.h"
#include "Enemy.h"

// Collision volumes that control that we are passing through checkpoints in the correct order.
#include "CP1.h"
#include "CP2.h"
#include "CP3.h"
#include "CP4.h"
#include "CP5.h"
#include "CP6.h"
#include "CP7.h"
#include "CP8.h"
#include "TimeAttackGoal.h"

#include "DrawDebugHelpers.h"
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
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"


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

	// ESC key Values
	bESCDown = false;

	// Defining Physics-related Values
	AngularDamping = 5.f;
	LinearDamping = 3.f;
	ForwardForce = 4000.f;
	TurnTorque = 20000.f;
	HoverForce = 350.f;
	HoverLength = 100.f;
	TraceLength = 130;

	// Defining Time related Values
	Millisecond = 0;
	Second = 0;
	Minute = 0;
	CountDownSeconds = 5.f;
	SetCountingDown();

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
	SpringArm->CameraLagSpeed = 20.f;
	SpringArm->SetupAttachment(PlayerMesh);

	// Creating & attaching Camera
	RearCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RearCamera->SetRelativeRotation(FRotator(0.f, 15.f, 0.f));
	RearCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Creating & attaching the Hover Components
	HoverComponentFL = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentFL"));
	HoverComponentFL->SetupAttachment(PlayerMesh);

	HoverComponentFR = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentFR"));
	HoverComponentFR->SetupAttachment(PlayerMesh);

	HoverComponentHL = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentHL"));
	HoverComponentHL->SetupAttachment(PlayerMesh);

	HoverComponentHR = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponentHR"));
	HoverComponentHR->SetupAttachment(PlayerMesh);

	TimerDel.BindUFunction(this, FName("LoadGame"), true);

	ExhaustR = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExhaustR"));
	ExhaustR->AttachTo(PlayerMesh);

	ExhaustL = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExhaustL"));
	ExhaustL->AttachTo(PlayerMesh);


}

// Called when the game starts or when spawned
void APlayerCar::BeginPlay()
{
	Super::BeginPlay();

	// Rotates camera to where the player car looks
	YaValue = GetActorRotation().Yaw;


	// Restriction physics
	PlayerMesh->SetAngularDamping(AngularDamping);
	//PlayerMesh->SetLinearDamping(LinearDamping);

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

	ExhaustR->Deactivate();
	ExhaustL->Deactivate();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerController* TempController = PlayerController;

	APlayerCar::DisableInput(PlayerController);
}

// Called every frame
void APlayerCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the Nitro is on, increase the forward force, else, reduce it back 
	if (bNitro) {
		if (NitroTime > 0) {
			NitroTime -= DeltaTime;
			SpringArm->CameraLagSpeed = 10.f;
			ExhaustR->Activate();
			ExhaustL->Activate();
		}
		else {
			bNitro = false;
			NitroTime = 0;
			ForwardForce /= 1.3f;
			SpringArm->CameraLagSpeed = 20.f;
			ExhaustR->Deactivate();
			ExhaustL->Deactivate();
		}
	}

	HealthFunction();

	// Checking Surface Normal
	Raycast();

	// Time, countown and similarities
	if (bCountingDown)
	{
		CountDownSeconds -= DeltaTime;
		if (CountDownSeconds <= 0.f)
		{
			CountDownSeconds = 0.f;
			bCountingDown = false;

			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			APlayerController* TempController = PlayerController;

			APlayerCar::EnableInput(TempController);
		}
	}

	if (!bCountingDown) {
		// Time
		Millisecond += DeltaTime * 1000;
		if (Millisecond >= 1000) {
			Millisecond -= 1000;
			Second++;
			if (Second >= 60) {
				Second -= 60;
				Minute++;
			}
		}
	}

	// Bring up victory screen
	if (CurrentLap == 4)
	{
		Winner();
	}

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
	PlayerInputComponent->BindAction("Target", EInputEvent::IE_Pressed, this, &APlayerCar::SetTarget);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &APlayerCar::Reload);
	PlayerInputComponent->BindAction("Nitro", EInputEvent::IE_Pressed, this, &APlayerCar::Nitro);
	PlayerInputComponent->BindAction("ChangeCameraView", EInputEvent::IE_Pressed, this, &APlayerCar::ChangeCameraView);

	PlayerInputComponent->BindAction("ESC", EInputEvent::IE_Pressed, this, &APlayerCar::ESCDown).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("ESC", EInputEvent::IE_Released, this, &APlayerCar::ESCUp).bExecuteWhenPaused = true;

	PlayerInputComponent->BindAction("Destroy", IE_Pressed, this, &APlayerCar::KillPlayer);
	PlayerInputComponent->BindAction("Minus", IE_Pressed, this, &APlayerCar::HealthMinus);

}

void APlayerCar::MoveForward(float Value)
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
				//if (TargetMesh)
				//{
					Energy--;
					FVector Location = GetActorLocation();
					FRotator Rotation = FRotator(PiValue + RearCamera->GetRelativeRotation().Pitch, YaValue, 0.f);

					if (ActorToSpawn->GetName() == "Bullet_BP_C")
					{
						World->SpawnActor<AActor>(ActorToSpawn, Location + Rotation.RotateVector(FVector(350.f, 0.f, 80.f)), Rotation);
						UE_LOG(LogTemp, Warning, TEXT("Firing Cannon Shell"));
					}
					else 
					{
						AHomingProjectile* HomingProjectile = World->SpawnActor<AHomingProjectile>(ActorToSpawn, Location + Rotation.RotateVector(FVector(350.f, 0.f, 80.f)), Rotation);
						if (HomingProjectile)
						{
							HomingProjectile->ProjectileMovement->HomingTargetComponent = TargetMesh;
							//TargetMesh->SetRenderCustomDepth(false);
							TargetMesh = nullptr;
							UE_LOG(LogTemp, Warning, TEXT("Firing Homing Missile"));
						}
					}
					UGameplayStatics::PlaySound2D(World, Shooting, 1.f, 1.f, 0.f, 0);
				//}
				//else 
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("ERROR: TargetMesh is FALSE"));
				//}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ERROR: World is FALSE"));
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
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: ActorToSpawn is NULL"));
	}
	if (HomingTarget)
	{
		HomingTarget->SetRenderCustomDepth(false);
		HomingTarget = nullptr;
	}
}

void APlayerCar::Reload() {
	Energy = MaxEnergy;
	UWorld* NewWorld = GetWorld();
	UGameplayStatics::PlaySound2D(NewWorld, Reloading, 1.f, 1.f, 0.f, 0);
}

void APlayerCar::SetTarget()
{
	TargetMesh = Target();
	//TargetMesh->SetRenderCustomDepth(true);
}

UStaticMeshComponent* APlayerCar::Target()
{
	FHitResult OutHit;
	FVector Start = RearCamera->GetComponentLocation();
	FVector End = Start + (RearCamera->GetForwardVector() * 5000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray; // object types to trace
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));

	TArray< AActor* > ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool Hit = (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, objectTypesArray, true, ActorsToIgnore,
		EDrawDebugTrace::Type::ForDuration, OutHit, true));

	UStaticMeshComponent* FirstHomingTarget = Cast<UStaticMeshComponent>(OutHit.GetComponent());

	if (Hit) {
		if (FirstHomingTarget)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Hit")));
			FirstHomingTarget->SetRenderCustomDepth(true);
			HomingTarget = FirstHomingTarget;
		}
		else
		{
			//FirstHomingTarget->SetRenderCustomDepth(false);
			HomingTarget = nullptr;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Not A PhysicsBody ")));
		}
	}
	else
	{
		if (HomingTarget) {
			HomingTarget->SetRenderCustomDepth(false);
			HomingTarget = nullptr;
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("No Hit")));
		}
	}
	return HomingTarget;
}

void APlayerCar::Nitro() {
	if (bNitro == false && Energy >= 2) {
		bNitro = true;
		Energy -= 2;

		NitroTime = 3.f;
		ForwardForce *= 1.3f;
	}
}

void APlayerCar::ChangeCameraView()
{
	YaValue += 180.f;
}

void APlayerCar::ESCDown()
{
	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	bESCDown = true;

	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}

void APlayerCar::ESCUp()
{
	bESCDown = false;
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

void APlayerCar::KillPlayer()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	//ParticleFX:
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetTransform(), true);

	//SoundFX
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerController* TempController = PlayerController;

	APlayerCar::DisableInput(PlayerController);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 3.f, false);

	//APlayerCar::EnableInput(TempController);
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
	}

	if (OtherActor->IsA(ACheckPoint::StaticClass()))
	{
		SaveGame();
	}

	if (OtherActor->IsA(AOutOfBoundsVolume::StaticClass()))
	{
		LoadGame(true);
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
		CurrentCheckpoint += 1;
	}

	if (OtherActor->IsA(ACP4::StaticClass()) && CurrentCheckpoint == 3)
	{
		CurrentCheckpoint += 1;
	}

	if (OtherActor->IsA(ACP5::StaticClass()) && CurrentCheckpoint == 4)
	{
		CurrentCheckpoint += 1;
	}

	if (OtherActor->IsA(ACP6::StaticClass()) && CurrentCheckpoint == 5)
	{
		CurrentCheckpoint += 1;
	}

	if (OtherActor->IsA(ACP7::StaticClass()) && CurrentCheckpoint == 6)
	{
		CurrentCheckpoint += 1;
	}

	if (OtherActor->IsA(ACP8::StaticClass()) && CurrentCheckpoint == 7)
	{
		CurrentCheckpoint = 0;
	}

	if (OtherActor->IsA(ATimeAttackGoal::StaticClass()) && CurrentCheckpoint == 7)
	{
		WinnerTimeAttack();
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

void APlayerCar::SaveGame()
{
	URacingSaveGame* SaveGameInstance = Cast<URacingSaveGame>(UGameplayStatics::CreateSaveGameObject(URacingSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Energy = Energy;
	SaveGameInstance->CharacterStats.MaxEnergy = MaxEnergy;
	SaveGameInstance->CharacterStats.Armour = Armour;
	SaveGameInstance->CharacterStats.MaxArmour = MaxArmour;

	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void APlayerCar::LoadGame(bool SetPosition)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerController* TempController = PlayerController;

	APlayerCar::EnableInput(TempController);

	URacingSaveGame* LoadGameInstance = Cast<URacingSaveGame>(UGameplayStatics::CreateSaveGameObject(URacingSaveGame::StaticClass()));

	LoadGameInstance = Cast<URacingSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Energy = LoadGameInstance->CharacterStats.Energy;
	MaxEnergy = LoadGameInstance->CharacterStats.MaxEnergy;
	Armour = LoadGameInstance->CharacterStats.Armour;
	MaxArmour = LoadGameInstance->CharacterStats.MaxArmour;

	if (SetPosition)
	{
		SetActorLocation((LoadGameInstance->CharacterStats.Location)+(FVector(0, 0, 100.f)));
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	}
}

void APlayerCar::HealthFunction()
{
	if (Health <= 0)
	{
		Health = 0.01;
		KillPlayer();
	}
}

void APlayerCar::HealthMinus()
{
	Health -= 10;
}

bool APlayerCar::IsCountingDown()
{
	return bCountingDown;
}

void APlayerCar::SetCountingDown()
{
	bCountingDown = true;
}

float APlayerCar::GetCountDownSeconds()
{
	return CountDownSeconds;
}

void APlayerCar::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void APlayerCar::TimeAttackLevel()
{
	UGameplayStatics::OpenLevel(this,"Level_TimeAttack", false);
}

void APlayerCar::RacingLevel()
{
	UGameplayStatics::OpenLevel(this, "Level_Race", false);
}

void APlayerCar::Winner()
{
	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (MainPlayerController)
	{
		MainPlayerController->DisplayWinScreen();
	}
}

void APlayerCar::WinnerTimeAttack()
{
	WinnerMillisecond = Millisecond;
	WinnerSecond = Second;
	WinnerMinute = Minute;


	if ((WinnerMinute <= 1) && (WinnerSecond <= 25))
	{
	Medal = (TEXT("GOLD"));
	}

	else if ((WinnerMinute <= 1) && (WinnerSecond <= 40) && (WinnerSecond > 25))
	{
		Medal = (TEXT("SILVER"));
	}

	else
	{
		Medal = (TEXT("BRONZE"));
	}

	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (MainPlayerController)
	{
		MainPlayerController->DisplayTimeAttackWinScreen();
	}
}
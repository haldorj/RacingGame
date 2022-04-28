// Fill out your copyright notice in the Description page of Project Settings.

#include "HomingProjectile.h"
#include "PlayerCar.h"
#include "Enemy.h"
#include "CheckPoint.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h" 

// Sets default values
AHomingProjectile::AHomingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 10.f;

	// Construct Collision Component
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	// Construct Static Mesh Component
	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh"));
	MissileMesh->SetupAttachment(RootComponent);

	// Construct Projectile Movement Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1800.f;
	ProjectileMovement->MaxSpeed = 1800.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 15000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Bind our OnOverlapBegin Event
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AHomingProjectile::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AHomingProjectile::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AHomingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHomingProjectile::TargetToHit(UStaticMeshComponent* Target)
{
	UStaticMeshComponent* HomingTarget = Target;

	if (HomingTarget != nullptr)
	{
		ProjectileMovement->HomingTargetComponent = HomingTarget;
	}
}

void AHomingProjectile::Explode()
{
	// ParticleFX:
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetTransform(), true);

	// SoundFX
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

	// Destroy projectile
	Destroy();
}

void AHomingProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(OtherActor->GetRootComponent());
	FVector Up = this->GetActorUpVector();
	FVector Forward = this->GetActorForwardVector();

	if ((OverlappedComp != nullptr) && (OtherActor != nullptr) && (OtherActor != this)
		&& (!OtherActor->IsA(APlayerCar::StaticClass())) && (!OtherActor->IsA(ACheckPoint::StaticClass())))
	{
		Explode();
	}

	if (OtherActor->IsA(AEnemy::StaticClass()))
	{	
		if (MeshComponent)
		{
			MeshComponent->AddImpulse((Up*0.25 + Forward)* ImpulseForce * MeshComponent->GetMass());
			UE_LOG(LogTemp, Warning, TEXT("FOUND MESH"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO MESH"));
		}
	Explode();
	}
}


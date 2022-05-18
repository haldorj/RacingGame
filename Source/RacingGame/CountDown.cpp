// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDown.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Containers/Array.h"
#include <UObject/ConstructorHelpers.h>

// Sets default values
ACountDown::ACountDown()
{
    Lights.Init(NULL, 3);

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CountdownNumber"));
    CountdownText->SetHorizontalAlignment(EHTA_Center);
    CountdownText->SetWorldSize(250.0f);
    CountdownText->SetTextRenderColor(FColor::Red);
    RootComponent = CountdownText;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshCom(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));



    Lights[0] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("light1"));
    Lights[1] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("light2"));
    Lights[2] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("light3"));

    int ArraySize = Lights.Num();
    if (SphereMeshCom.Succeeded()) {
        for (int i = 0; i < ArraySize; i++) {
            Lights[i]->SetStaticMesh(SphereMeshCom.Object);
            UE_LOG(LogTemp, Warning, TEXT("check"));
        }
    }

    CountdownTime = 3;

}

// Called when the game starts or when spawned
void ACountDown::BeginPlay()
{
    Super::BeginPlay();
    Lights[0]->SetWorldLocation(FVector(760.f, -200.f, 350.f));
    Lights[1]->SetWorldLocation(FVector(760.f, 0.f, 350.f));
    Lights[2]->SetWorldLocation(FVector(760.f, 200.f, 350.f));

    UpdateTimerDisplay();
    GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ACountDown::AdvanceTimer, 1.0f, true);
    UWorld* World = GetWorld();
    if (World) {
        UGameplayStatics::PlaySound2D(World, CountDownSound, 1.f, 1.f, 0.f);
    }
}

// Called every frame
void ACountDown::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ACountDown::UpdateTimerDisplay()
{
    CountdownText->SetText(FText::AsNumber(CountdownTime));

}

void ACountDown::AdvanceTimer()
{

    UpdateTimerDisplay();
    if (CountdownTime < 1)
    {
        CountdownHasFinished();

    }
    if (CountdownTime < 0)
    {
        //We're done counting down, so stop running the timer.
        GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
        Destroy();

    }
    CountdownTime--;
    if (CountdownTime >= 0 && CountdownTime < 3) {
        Lights[CountdownTime]->DestroyComponent();
    }
}

void ACountDown::CountdownHasFinished()
{
    //Change to a special readout
    APlayerCar* PlayerCar = Cast<APlayerCar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    PlayerCar->bGameCanPlay = true;
    CountdownText->SetTextRenderColor(FColor::Green);
    CountdownText->SetText(INVTEXT("GO!")); //INVTEXT == text that does not need translation / localization



}
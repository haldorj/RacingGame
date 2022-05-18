// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"



void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WWinScreen)
	{
		WinScreen = CreateWidget<UUserWidget>(this, WWinScreen);
		if (WinScreen)
		{
			WinScreen->AddToViewport();
			WinScreen->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WLoseScreen)
	{
		LoseScreen = CreateWidget<UUserWidget>(this, WLoseScreen);
		if (LoseScreen)
		{
			LoseScreen->AddToViewport();
			LoseScreen->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WTimeAttackWinScreen)
	{
		TimeAttackWinScreen = CreateWidget<UUserWidget>(this, WTimeAttackWinScreen);
		if (TimeAttackWinScreen)
		{
			TimeAttackWinScreen->AddToViewport();
			TimeAttackWinScreen->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		FInputModeGameOnly InputModeGameOnly;

		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;

		bPauseMenuVisible = false;

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::DisplayWinScreen_Implementation()
{
	if (WinScreen)
	{
		WinScreen->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AMainPlayerController::RemoveWinScreen_Implementation()
{
	if (WinScreen)
	{
		FInputModeGameOnly InputModeGameOnly;

		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;

		bPauseMenuVisible = false;

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void AMainPlayerController::DisplayLoseScreen_Implementation()
{
	if (LoseScreen)
	{
		LoseScreen->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AMainPlayerController::RemoveLoseScreen_Implementation()
{
	if (LoseScreen)
	{
		LoseScreen->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AMainPlayerController::DisplayTimeAttackWinScreen_Implementation()
{
	if (TimeAttackWinScreen)
	{
		TimeAttackWinScreen->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AMainPlayerController::RemoveTimeAttackWinScreen_Implementation()
{
	if (TimeAttackWinScreen)
	{
		FInputModeGameOnly InputModeGameOnly;

		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;

		bPauseMenuVisible = false;

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}
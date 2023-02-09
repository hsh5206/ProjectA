// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Main/MainPlayerController.h"
#include "HUD/MainWidget.h"
#include "Components/ProgressBar.h"

AMainPlayerController::AMainPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainWidget> widgetClass(TEXT("/Game/Blueprint/Widget/WBP_Main.WBP_Main_C"));
	if (widgetClass.Succeeded())
	{
		MainWidgetClass = widgetClass.Class;
	}
}

void AMainPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AMainPlayerController::SetStaminaPercent(float Max, float Cur)
{
	if (MainWidget)
	{
		MainWidget->StaminaBar->SetPercent(Cur / Max);
	}
}

void AMainPlayerController::SetHealthPercent(float Max, float Cur)
{
	if (MainWidget)
	{
		MainWidget->HealthBar->SetPercent(Cur / Max);
	}
}

void AMainPlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	Super::BeginPlay();
	if (MainWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("MainWidgetClass"));

		MainWidget = CreateWidget<UMainWidget>(this, MainWidgetClass);
		MainWidget->AddToViewport();
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

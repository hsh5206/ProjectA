// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	AMainPlayerController();
	UPROPERTY()
	class UMainWidget* MainWidget;
	UPROPERTY()
	TSubclassOf<class UMainWidget> MainWidgetClass;

	void SetStaminaPercent(float Max, float Cur);
	void SetHealthPercent(float Max, float Cur);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};

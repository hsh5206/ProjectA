// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/Main/MainCharacterState.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class AMainCharacter* MainCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* MainMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Direction;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = "Character State")
	ECharacterArmedState CharacterState;
};

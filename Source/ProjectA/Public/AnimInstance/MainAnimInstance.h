// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "Characters/Main/MainCharacterState.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UMainAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class AMainCharacter* MainCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Character State")
	ECharacterArmedState CharacterState;
	UPROPERTY(BlueprintReadWrite, Category = "Character State")
	ECharacterCombatState CombatState;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "Characters/Enemy/EnemyState.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UEnemyAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState;
	UPROPERTY(BlueprintReadOnly)
	class AEnemy* Enemy;
};

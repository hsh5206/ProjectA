// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AEnemy();
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;
};

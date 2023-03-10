// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyState.h"
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
	void Dead();
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	/** Montage */
	UPROPERTY(EditAnywhere)
	class UAnimMontage* DeadMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;

	/** Attack */
	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	/** State */
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Default;
	void SetEnemyState(EEnemyState state);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeapon> WeaponToSpawn;

protected:
	virtual void BeginPlay() override;
};

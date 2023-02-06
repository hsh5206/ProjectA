// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "MainCharacterState.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** State */
	UPROPERTY(VisibleAnywhere)
	bool bWalk = false;
	UPROPERTY(VisibleAnywhere)
	ECharacterArmedState MainState = ECharacterArmedState::EAS_Unarmed;
	UPROPERTY(VisibleAnywhere)
	ECharacterCombatState CombatState = ECharacterCombatState::ECS_Default;
	FCharacterRoll CharacterRollState;

	/** Montage */
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EquipMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;
	void PlayAttackMontage(FName SectionName);
	UPROPERTY(EditAnywhere)
	class UAnimMontage* RollMontage;

	/** Callbacks by AnimNotify */
	UFUNCTION(BlueprintCallable)
	void ChangeArmDsiarm();
	UFUNCTION(BlueprintCallable)
	void AttackStartComboState();
	UFUNCTION(BlueprintCallable)
	void AttackEndComboState();
	UFUNCTION(BlueprintCallable)
	void RollEnd();

	/** Sensing */
	UPROPERTY(EditAnywhere, Category = "Sensing")
	class USphereComponent* LockOnSphere;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	/** Callbacks by Input */
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void WalkRun();
	void ArmDisarm();
	void EPress();
	void LockOn();
	void Attack();
	void Roll();

	void UnLockOn();
	FName GetRollWay();

	/** LockOn Sphere */
	UFUNCTION()
	virtual void OnLockOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnLockOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	class AItem* OverlappedItem;
	UPROPERTY(VisibleAnywhere)
	class APawn* LockedOnEnemy;

	/** Combo Attack */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	int32 MaxCombo = 4;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentCombo;
	
	/** temp */
	UPROPERTY(VisibleAnywhere)
	class APawn* CanLockedOnEnemy;

public:
	FORCEINLINE void SetOverlappedItem(AItem* item) { OverlappedItem = item; }
};

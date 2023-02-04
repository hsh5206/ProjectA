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
	ECharacterState MainState = ECharacterState::EAS_Unarmed;

	/** Montage */
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EquipMontage;

	/** Callbacks by AnimNotify */
	UFUNCTION(BlueprintCallable)
	void ChangeArmDsiarm();

	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	/** Input Actions */
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void WalkRun();
	void ArmDisarm();
	void EPress();

private:
	UPROPERTY(VisibleAnywhere)
	class AItem* OverlappedItem;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class AWeapon* Weapon;

	

public:
	FORCEINLINE void SetOverlappedItem(AItem* item) { OverlappedItem = item; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECTA_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class AWeapon* Weapon;

	UFUNCTION(BlueprintCallable)
	void EnableWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(VisibleAnywhere)
	float MaxHealth;
	UPROPERTY(VisibleAnywhere)
	float Health;
	float MaxStamina;
	float Stamina;

	float GetHealthPercentage();

	/** Stat */
	float Vigor = 5; // �����
	float Endurance = 5; // ������
	float Power = 5; // ��
	float Agility = 5; // ��ø
	float Durability = 5; // ����
};

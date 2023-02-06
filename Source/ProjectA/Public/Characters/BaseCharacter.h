// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECTA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class AWeapon* Weapon;

	float MaxHealth;
	float Health;
	float MaxStamina;
	float Stamina;

	/** Stat */
	float Vigor = 5; // �����
	float Endurance = 5; // ������
	float Power = 5; // ��
	float Agility = 5; // ��ø
	float Durability = 5; // ����
};

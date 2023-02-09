// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Item/Weapon/Weapon.h"
#include "Components/BoxComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::EnableWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (Weapon)
	{
		Weapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		Weapon->IgnoreActors.Empty();
	}
}

float ABaseCharacter::GetHealthPercentage()
{
	return Health / MaxHealth;
}

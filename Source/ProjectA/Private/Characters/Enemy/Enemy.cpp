// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Enemy.h"
#include "HUD/HealthBarComponent.h"

AEnemy::AEnemy()
{
	MaxHealth = 100;
	Health = 100;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	DrawDebugSphere(GetWorld(), ImpactPoint, 8.f, 12, FColor::Green, false, 5.f);
	UE_LOG(LogTemp, Warning, TEXT("Enemy::GetHit"));
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), DamageAmount);
	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(GetHealthPercentage());
	}
	return DamageAmount;
}

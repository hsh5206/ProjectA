// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Enemy.h"
#include "HUD/HealthBarComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Enemy/EnemyAIController.h"
#include "Item/Weapon/Weapon.h"

AEnemy::AEnemy()
{
	MaxHealth = 100;
	Health = 100;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Weapon = GetWorld()->SpawnActor<AWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (Weapon)
	{
		Weapon->Equip(GetMesh(), FName("RightHandSocket"), this);
		Weapon->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	DrawDebugSphere(GetWorld(), ImpactPoint, 8.f, 12, FColor::Green, false, 5.f);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), DamageAmount);
	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(GetHealthPercentage());
	}
	if (Health == 0.f)
	{
		EnemyState = EEnemyState::EES_Dead;
		Dead();
	}
	return DamageAmount;
}

void AEnemy::Dead()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeadMontage)
	{
		AnimInstance->Montage_Play(DeadMontage);
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthBarWidget->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
}

void AEnemy::Attack()
{
	if (EnemyState == EEnemyState::EES_Dead) return;
	if (EnemyState == EEnemyState::EES_Attacking) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		if (!AttackFrontHandle.IsValid())
		{
			AnimInstance->Montage_JumpToSection(FName("AttackFront"), AttackMontage);
			GetWorldTimerManager().SetTimer(AttackFrontHandle, this, &AEnemy::FrontTimerEnd, AttackFrontCoolTime);
		}
		else if (!AttackBackHandle.IsValid())
		{
			AnimInstance->Montage_JumpToSection(FName("AttackBack"), AttackMontage);
			GetWorldTimerManager().SetTimer(AttackBackHandle, this, &AEnemy::BackTimerEnd, AttackBackCoolTime);

		}
		EnemyState = EEnemyState::EES_Attacking;
	}
}

void AEnemy::FrontTimerEnd()
{
	GetWorldTimerManager().ClearTimer(AttackFrontHandle);
}

void AEnemy::BackTimerEnd()
{
	GetWorldTimerManager().ClearTimer(AttackBackHandle);
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_Default;
}

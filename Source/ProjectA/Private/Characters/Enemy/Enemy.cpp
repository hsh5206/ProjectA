// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Enemy.h"
#include "HUD/HealthBarComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Enemy/EnemyAIController.h"
#include "Item/Weapon/Weapon.h"
#include "Characters/Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	/*Weapon = GetWorld()->SpawnActor<AWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (Weapon)
	{
		Weapon->Equip(GetMesh(), FName("RightHandSocket"), this);
		Weapon->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}*/

	if (WeaponToSpawn)
	{
		Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponToSpawn));
		Weapon->Equip(GetMesh(), FName("RightHandSocket"), this);
		Weapon->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactPoint);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(GetHealthPercentage());
	}
	if (Health == 0.f)
	{
		SetEnemyState(EEnemyState::EES_Dead);
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
	auto controllerRef = GetController();
	GetController()->UnPossess();
	controllerRef->Destroy();
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
		SetEnemyState(EEnemyState::EES_Attacking);
		int32 Section = FMath::RandRange(1, 3);
		
		AnimInstance->Montage_JumpToSection(FName(*FString::Printf(TEXT("Attack%d"), Section)), AttackMontage);
	}
}

void AEnemy::AttackEnd()
{
	SetEnemyState(EEnemyState::EES_Default);
}

void AEnemy::SetEnemyState(EEnemyState state)
{
	EnemyState = state;
	/*AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::Key_EnemyState, (uint8)state);
	}*/
}

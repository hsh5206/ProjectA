// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Task/BTT_Turn.h"
#include "Characters/Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Main/MainCharacter.h"
#include "Characters/Enemy/Enemy.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_Turn::UBTT_Turn()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Turn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	AEnemy* Enemy = Cast<AEnemy>(ControllingPawn);
	if (Enemy->EnemyState == EEnemyState::EES_Dead) return EBTNodeResult::Failed;

	AMainCharacter* Character = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Key_Target));
	if (nullptr == Character) return EBTNodeResult::Failed;

	const FRotator CharacterRotation = ControllingPawn->GetActorRotation();
	const FRotator Direction = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), Character->GetActorLocation());
	ControllingPawn->SetActorRotation(FRotator(CharacterRotation.Pitch, Direction.Yaw, CharacterRotation.Roll));

	return EBTNodeResult::Succeeded;
}
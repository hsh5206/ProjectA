// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Task/BTT_Attack.h"
#include "Characters/Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Main/MainCharacter.h"
#include "Characters/Enemy/Enemy.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	AMainCharacter* Character = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Key_Target));
	if (nullptr == Character) return EBTNodeResult::Failed;

	if (AEnemy* Enemy = Cast<AEnemy>(ControllingPawn))
	{
		Enemy->Attack();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

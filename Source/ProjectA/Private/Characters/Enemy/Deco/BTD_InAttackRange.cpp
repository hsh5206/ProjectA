// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Deco/BTD_InAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Main/MainCharacter.h"
#include "NavigationSystem.h"
#include "Characters/Enemy/EnemyAIController.h"

UBTD_InAttackRange::UBTD_InAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTD_InAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return false;

	AMainCharacter* Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Key_Target));
	if (nullptr == Target) return false;

	bool bResult = (Target->GetDistanceTo(ControllingPawn) <= 100.0f);
	return bResult;
}

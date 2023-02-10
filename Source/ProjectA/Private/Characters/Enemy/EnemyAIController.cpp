// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyAIController::Key_HomePos(TEXT("HomePos"));
const FName AEnemyAIController::Key_PatrolPos(TEXT("PatrolPos"));
const FName AEnemyAIController::Key_Target(TEXT("Target"));

AEnemyAIController::AEnemyAIController()
{
	BlackboardComponent = Blackboard.Get();

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprint/Enemies/AI/BB_EnemyAI.BB_EnemyAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprint/Enemies/AI/BT_EnemyAI.BT_EnemyAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(Key_HomePos, InPawn->GetActorLocation());
		RunBehaviorTree(BTAsset);
	}
}
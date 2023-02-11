// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Characters/Main/MainCharacter.h"
#include "Characters/Enemy/Enemy.h"

const FName AEnemyAIController::Key_HomePos(TEXT("HomePos"));
const FName AEnemyAIController::Key_PatrolPos(TEXT("PatrolPos"));
const FName AEnemyAIController::Key_Target(TEXT("Target"));
const FName AEnemyAIController::Key_bDetected(TEXT("bDetected"));
const FName AEnemyAIController::Key_EnemyState(TEXT("EEnemyState"));

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
	
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(Key_HomePos, InPawn->GetActorLocation());
		RunBehaviorTree(BTAsset);
	}

	Enemy = Cast<AEnemy>(InPawn);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Blackboard->SetValueAsEnum(Key_EnemyState, (uint8)Enemy->EnemyState);
}

void AEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (auto const target = Cast<AMainCharacter>(actor))
	{
		Blackboard->SetValueAsBool(Key_bDetected, Stimulus.WasSuccessfullySensed());
		Blackboard->SetValueAsObject(Key_Target, target);

		UE_LOG(LogTemp, Warning, TEXT("!!!!!!"));
	}
}

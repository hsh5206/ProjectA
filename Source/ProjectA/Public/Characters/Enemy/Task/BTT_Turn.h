// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Turn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UBTT_Turn : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_Turn();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

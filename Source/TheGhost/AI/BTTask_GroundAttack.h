// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GroundAttack.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API UBTTask_GroundAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GroundAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckBossHP.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API UBTDecorator_CheckBossHP : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckBossHP();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Condition")
	float HealthThreshold;
};

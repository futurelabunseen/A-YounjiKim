// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckBossHP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TGAI.h"

UBTDecorator_CheckBossHP::UBTDecorator_CheckBossHP()
{
	NodeName = "IF Boss HP";
	HealthThreshold = 100.0f; // ±âº»°ª
}

bool UBTDecorator_CheckBossHP::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    if (OwnerComp.GetBlackboardComponent() == nullptr)
    {
        return false;
    }

    float CurrentHealth = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_BOSSHP);

    return CurrentHealth <= HealthThreshold;
}

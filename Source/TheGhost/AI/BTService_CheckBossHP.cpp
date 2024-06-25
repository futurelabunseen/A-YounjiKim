// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckBossHP.h"
#include "TGAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/TGCharacterBoss.h"

UBTService_CheckBossHP::UBTService_CheckBossHP()
{
	NodeName = "Check Boss HP";
	Interval = 1.0f;
}

void UBTService_CheckBossHP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return;
	}

	ATGCharacterBoss* AIPawn = Cast<ATGCharacterBoss>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float CurrentHealth = AIPawn->GetCurrentHp();
	if (!ensure(CurrentHealth >= 0.0f))
	{
		return;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_BOSSHP, CurrentHealth);
}

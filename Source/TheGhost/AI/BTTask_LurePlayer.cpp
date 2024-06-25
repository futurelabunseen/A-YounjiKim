// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_LurePlayer.h"
#include "AIController.h"
#include "Interface/TGCharacterAIInterface.h"
#include "TGAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LurePlayer::UBTTask_LurePlayer()
{
	NodeName = "Lure Player";
}

EBTNodeResult::Type UBTTask_LurePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ITGCharacterAIInterface* AIPawn = Cast<ITGCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AIPawn->SetAIAttackDelegate(OnAttackFinished);

	TObjectPtr<class AActor> Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target)
	{
		AIPawn->AttackByAI(EAttackType::Lure, Target);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISLURED, true);
	}
	return EBTNodeResult::Type();
}

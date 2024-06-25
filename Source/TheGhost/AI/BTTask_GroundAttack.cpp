// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_GroundAttack.h"
#include "AIController.h"
#include "Interface/TGCharacterAIInterface.h"
#include "TGAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GroundAttack::UBTTask_GroundAttack()
{
}

EBTNodeResult::Type UBTTask_GroundAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	// ���� ������ ǥ��
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISATTACK, true);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// AI�� ����ִ� Pawn
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

	// Target = �÷��̾�
	TObjectPtr<class AActor> Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target)
	{
		AIPawn->AttackByAI(EAttackType::Ground, Target); // Ground Attack �������� �̵�
	}
	
	// Ground Attack�� �� ���� �����ϱ� ���� false�� ����
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CANGROUNDATTACK, false);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISATTACK, false);
	return EBTNodeResult::InProgress;
}


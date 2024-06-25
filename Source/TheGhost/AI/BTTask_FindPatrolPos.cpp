// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "TGAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/TGCharacterAIInterface.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // 컴포넌트를 갖고 있는 폰 얻어오기
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	ITGCharacterAIInterface* AIPawn = Cast<ITGCharacterAIInterface>(ControllingPawn); // 의존역전법칙
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	float PatrolRadius = AIPawn->GetAIPatrolRadius();
	FNavLocation NextPatrolPos;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos)) // PatrolRadius 크기만큼의 반경 내에 랜덤한 위치값을 찾으면 true
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	/*ITGCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}*/

	return EBTNodeResult::Failed;
}

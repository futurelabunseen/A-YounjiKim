// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "TGAI.h"
#include "AIController.h"
#include "Interface/TGCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/TGCollision.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect"); // 노드 이름이 Detect라고 표시되도록 지정
	Interval = 1.0f; // Service의 Tick이 실행되는 간격을 1초로
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // 지금 빙의된 폰의 정보를 가져온다. = NPC
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation(); // 폰의 위치
	UWorld* World = ControllingPawn->GetWorld(); // 월드 정보
	if (nullptr == World)
	{
		return;
	}

	ITGCharacterAIInterface* AIPawn = Cast<ITGCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange(); // 감지 범위 얻어오기

	TArray<FOverlapResult> OverlapResults; 
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel( // 충돌 설정
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_TGACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult) // 충돌이 감지된다면
	{
		for (auto const& OverlapResult : OverlapResults) // 결과값 하나씩 흝으면서 모든
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			// 폰을 지금 조종하고 있는 게 플레이어 컨트롤러인 경우, 즉 캐릭터 플레이어인 경우만
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// 블랙보드의 Target 값에 감지된 Pawn 할당
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				return;
			}
		}
	}
	// 찾지 못한 경우
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
}



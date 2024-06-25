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
	NodeName = TEXT("Detect"); // ��� �̸��� Detect��� ǥ�õǵ��� ����
	Interval = 1.0f; // Service�� Tick�� ����Ǵ� ������ 1�ʷ�
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // ���� ���ǵ� ���� ������ �����´�. = NPC
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation(); // ���� ��ġ
	UWorld* World = ControllingPawn->GetWorld(); // ���� ����
	if (nullptr == World)
	{
		return;
	}

	ITGCharacterAIInterface* AIPawn = Cast<ITGCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange(); // ���� ���� ������

	TArray<FOverlapResult> OverlapResults; 
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel( // �浹 ����
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_TGACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult) // �浹�� �����ȴٸ�
	{
		for (auto const& OverlapResult : OverlapResults) // ����� �ϳ��� �y���鼭 ���
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			// ���� ���� �����ϰ� �ִ� �� �÷��̾� ��Ʈ�ѷ��� ���, �� ĳ���� �÷��̾��� ��츸
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// �������� Target ���� ������ Pawn �Ҵ�
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				return;
			}
		}
	}
	// ã�� ���� ���
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
}



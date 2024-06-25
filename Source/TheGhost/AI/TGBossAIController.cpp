// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TGBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TGAI.h"

ATGBossAIController::ATGBossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/TheGhost/AI/BT_TGBoss.BT_TGBoss'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/TheGhost/AI/BB_TGCharacter.BB_TGCharacter'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}
}

void ATGBossAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get(); // 블랙보드 포인터 얻어오는 함수 Get

	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation()); // 현재 클래스를 담고 있는 Pawn의 루트컴포넌트의 좌표값을 블랙보드의 HomePos에 설정. 
		Blackboard->SetValueAsFloat(BBKEY_BOSSHP, 100.f);
		Blackboard->SetValueAsBool(BBKEY_CANGROUNDATTACK, true);

		bool RunResult = RunBehaviorTree(BTAsset); // BT 실행
		ensure(RunResult);
	}
}

void ATGBossAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent); // 현재 동작하고 있는 BT 가져오기. BrainComponent는 AI 로직을 위한 Base 클래스.
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ATGBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI(); // 폰이 빙의되었을 때 AI 실행
}

void ATGBossAIController::OnUnPossess()
{
	Super::OnUnPossess();

	StopAI();
}
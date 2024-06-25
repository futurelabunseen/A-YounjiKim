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
	UBlackboardComponent* BlackboardPtr = Blackboard.Get(); // ������ ������ ������ �Լ� Get

	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation()); // ���� Ŭ������ ��� �ִ� Pawn�� ��Ʈ������Ʈ�� ��ǥ���� �������� HomePos�� ����. 
		Blackboard->SetValueAsFloat(BBKEY_BOSSHP, 100.f);
		Blackboard->SetValueAsBool(BBKEY_CANGROUNDATTACK, true);

		bool RunResult = RunBehaviorTree(BTAsset); // BT ����
		ensure(RunResult);
	}
}

void ATGBossAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent); // ���� �����ϰ� �ִ� BT ��������. BrainComponent�� AI ������ ���� Base Ŭ����.
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ATGBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI(); // ���� ���ǵǾ��� �� AI ����
}

void ATGBossAIController::OnUnPossess()
{
	Super::OnUnPossess();

	StopAI();
}
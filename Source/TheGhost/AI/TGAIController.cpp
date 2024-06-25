// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TGAI.h"

ATGAIController::ATGAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/TheGhost/AI/BB_TGCharacter.BB_TGCharacter'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/TheGhost/AI/BT_TGCharacter.BT_TGCharacter'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ATGAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get(); // ������ ������ ������ �Լ� Get

	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation()); // ���� Ŭ������ ��� �ִ� Pawn�� ��Ʈ������Ʈ�� ��ǥ���� �������� HomePos�� ����. 

		bool RunResult = RunBehaviorTree(BTAsset); // BT ����
		ensure(RunResult);
	}
}

void ATGAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent); // ���� �����ϰ� �ִ� BT ��������. BrainComponent�� AI ������ ���� Base Ŭ����.
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ATGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI(); // ���� ���ǵǾ��� �� AI ����
}

void ATGAIController::OnUnPossess()
{
	Super::OnUnPossess();

	StopAI();
}

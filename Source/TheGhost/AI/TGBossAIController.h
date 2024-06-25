// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TGBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API ATGBossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATGBossAIController();

	virtual void RunAI();
	virtual void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override; // � ��Ʈ�ѷ��� ���� �����ؼ� ������ �� �� �߻��Ǵ� �̺�Ʈ �Լ�
	virtual void OnUnPossess() override;

protected:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};

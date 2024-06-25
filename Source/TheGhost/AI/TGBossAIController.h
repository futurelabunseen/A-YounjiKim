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
	virtual void OnPossess(APawn* InPawn) override; // 어떤 컨트롤러가 폰에 빙의해서 조종을 할 때 발생되는 이벤트 함수
	virtual void OnUnPossess() override;

protected:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};

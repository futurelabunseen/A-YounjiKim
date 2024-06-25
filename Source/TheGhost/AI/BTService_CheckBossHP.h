// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckBossHP.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API UBTService_CheckBossHP : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckBossHP();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/*UPROPERTY(EditAnywhere, Category = "Health")
	float HPThreshold;*/
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_UpdatePlayerLocation.h"
#include "AIController.h"
#include "Interface/TGCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdatePlayerLocation::UBTService_UpdatePlayerLocation()
{
	NodeName = TEXT("UpdatePlayer");
	Interval = 1.0f;
}

void UBTService_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

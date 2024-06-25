// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TGGASPlayerState.h"
#include "AbilitySystemComponent.h"

ATGGASPlayerState::ATGGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	 //멀티플레이어 기준 - ASC가 서버에서 클라이언트로 계속 전송되어야 함.(replication)
	//ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* ATGGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TGGASPlayerState.h"
#include "AbilitySystemComponent.h"

ATGGASPlayerState::ATGGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	 //��Ƽ�÷��̾� ���� - ASC�� �������� Ŭ���̾�Ʈ�� ��� ���۵Ǿ�� ��.(replication)
	//ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* ATGGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterNonPlayer.h"

ATGCharacterNonPlayer::ATGCharacterNonPlayer()
{
}

void ATGCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle; // ���� �ð��� ���� �� ������� �ϱ� ���� Ÿ�̸�
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda( // ���� �Լ��� �̿��Ͽ� Timer Delegate�� ��� �Լ��� ����
		[&]()
		{
			Destroy(); // ���� ���� �Լ�
		}
	), DeadEventDelayTime, false); // �θ� Ŭ������ �ִ� DelayTime�� �������ְ� loop�� X(false)
}

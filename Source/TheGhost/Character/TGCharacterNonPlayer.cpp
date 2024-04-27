// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterNonPlayer.h"

ATGCharacterNonPlayer::ATGCharacterNonPlayer()
{
}

void ATGCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle; // 일정 시간이 지난 후 사라지게 하기 위한 타이머
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda( // 람다 함수를 이용하여 Timer Delegate에 멤버 함수를 부착
		[&]()
		{
			Destroy(); // 액터 제공 함수
		}
	), DeadEventDelayTime, false); // 부모 클래스에 있는 DelayTime을 설정해주고 loop는 X(false)
}

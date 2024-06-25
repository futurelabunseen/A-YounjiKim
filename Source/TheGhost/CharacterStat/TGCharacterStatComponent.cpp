// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/TGCharacterStatComponent.h"
#include "GameData/TGGameSingleton.h"

// Sets default values for this component's properties
UTGCharacterStatComponent::UTGCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 100.0f;
	//MaxHp = 200.0f;
	//CurrentHp = MaxHp;
}


// Called when the game starts
void UTGCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
	
}

void UTGCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp); // OnHpChanged 라는 구조체에 등록된 객체나 함수들에게 변경된 CurrentHp 값을 넘겨준다.
}

void UTGCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UTGGameSingleton::Get().CharacterMaxLevel); // 레벨의 값은 1~싱글톤의MaxLevel값 사이여야 함
	BaseStat = UTGGameSingleton::Get().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f); // 제대로 잘 가져오는지 확인
}

void UTGCharacterStatComponent::InitializeAttack()
{
	ModifierStat.Attack = 0;
	BaseStat.Attack = 10;
}

float UTGCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage); // 데미지 양으로 -가 들어오지 않도록 범위 조절

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) // 현재 HP가 가장 작은 값이 된다면 사실상 죽은 상태
	{
		OnHpZero.Broadcast(); // 죽은 상태를 알림
	}

	return ActualDamage;
}


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

	OnHpChanged.Broadcast(CurrentHp); // OnHpChanged ��� ����ü�� ��ϵ� ��ü�� �Լ��鿡�� ����� CurrentHp ���� �Ѱ��ش�.
}

void UTGCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UTGGameSingleton::Get().CharacterMaxLevel); // ������ ���� 1~�̱�����MaxLevel�� ���̿��� ��
	BaseStat = UTGGameSingleton::Get().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f); // ����� �� ���������� Ȯ��
}

void UTGCharacterStatComponent::InitializeAttack()
{
	ModifierStat.Attack = 0;
	BaseStat.Attack = 10;
}

float UTGCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage); // ������ ������ -�� ������ �ʵ��� ���� ����

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) // ���� HP�� ���� ���� ���� �ȴٸ� ��ǻ� ���� ����
	{
		OnHpZero.Broadcast(); // ���� ���¸� �˸�
	}

	return ActualDamage;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/TGCharacterStat.h"
#include "TGCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate); // ���ڷ�, ����� ����ü �̸�
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGHOST_API UTGCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTGCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetHp(float NewHp); // HP�� ����� ������ �ݵ�� �� �Լ��� ȣ���ϵ���

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged; // ���⿡ �𸮾� ������Ʈ�� C++ �Լ�, �������Ʈ �Լ��� ��ϵ� ����.

	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void SetModifierStat(const FTGCharacterStat& InModifierStat) { ModifierStat = InModifierStat; } // ���� ��µ� �� ������̾� ���� ����
	FORCEINLINE FTGCharacterStat GetTotalStat() { return BaseStat + ModifierStat; } // ĳ���� ��ü ���� ��������
	FORCEINLINE void SetAttack(float newAttack) { ModifierStat.Attack = newAttack; }
	FORCEINLINE void InitializeAttack();
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamage); // �������� �޾��� �� �ش� ������ ����

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat) // VisibleInstanceOnly = ĳ���͵鸶�� ���� �ٸ� �� ���� ����, Transient = Disk�� ������ �ʿ䰡 ���� ������
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius; // ���� �� ������ �Ÿ��� ���� �� ����ϴ� ATGCharacterBase::AttackHitCheck()�� AttackRadius�� ���⼭ �����ϰԲ�

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FTGCharacterStat BaseStat; // �⺻ ����

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FTGCharacterStat ModifierStat; // ������̾� ���� (���ݷ� ����, ���� ���� �� �������� ����)
};

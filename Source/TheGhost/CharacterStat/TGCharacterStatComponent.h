// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/TGCharacterStat.h"
#include "TGCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate); // 인자로, 사용할 구조체 이름
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
	void SetHp(float NewHp); // HP가 변경될 때에는 반드시 이 함수를 호출하도록

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged; // 여기에 언리얼 오브젝트나 C++ 함수, 블루프린트 함수가 등록될 예정.

	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void SetModifierStat(const FTGCharacterStat& InModifierStat) { ModifierStat = InModifierStat; } // 방어력 상승될 때 모디파이어 스탯 변경
	FORCEINLINE FTGCharacterStat GetTotalStat() { return BaseStat + ModifierStat; } // 캐릭터 전체 스탯 가져오기
	FORCEINLINE void SetAttack(float newAttack) { ModifierStat.Attack = newAttack; }
	FORCEINLINE void InitializeAttack();
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamage); // 데미지를 받았을 때 해당 데미지 적용

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat) // VisibleInstanceOnly = 캐릭터들마다 각각 다른 값 설정 가능, Transient = Disk에 저장할 필요가 없는 데이터
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius; // 폰과 폰 사이의 거리를 구할 때 사용하는 ATGCharacterBase::AttackHitCheck()의 AttackRadius를 여기서 조절하게끔

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FTGCharacterStat BaseStat; // 기본 스탯

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FTGCharacterStat ModifierStat; // 모디파이어 스탯 (공격력 증가, 방어력 증가 등 유동적인 스탯)
};

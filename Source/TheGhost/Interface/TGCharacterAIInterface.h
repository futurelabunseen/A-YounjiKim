// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TGCharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTGCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAICharacterAttackFinished);

UENUM()
enum class EAttackType : uint8
{
	Basic,
	Ground,
	Lure
};

class THEGHOST_API ITGCharacterAIInterface
{
	GENERATED_BODY()

public:
	// NPC가 의무적으로 가져야할 기본적인 데이터를 얻어오는 함수들
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetHp(float NewHp) = 0;
	virtual float GetMaxHp() = 0;
	virtual float GetCurrentHp() = 0;
	virtual float ApplyDamage(float InDamage) = 0;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI(EAttackType AttackType, TObjectPtr<class AActor> Target) = 0;
};

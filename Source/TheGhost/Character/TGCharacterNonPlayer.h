// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/TGCharacterAIInterface.h"
#include "TGCharacterNonPlayer.generated.h"

class UTGHpBarWidget;
/**
 * 
 */
UCLASS()
class THEGHOST_API ATGCharacterNonPlayer : public ATGCharacterBase, public ITGCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	ATGCharacterNonPlayer();
	virtual float ApplyDamage(float InDamage) override;

protected:
	virtual void SetDead() override;
	virtual void PlayDeadAnimation() override;
	virtual void BasicAttack();

	// AI Section
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI(EAttackType AttackType, TObjectPtr<class AActor> Target) override;
	virtual void EndAttack(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	FAICharacterAttackFinished OnAttackFinished;

// Animation
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> BasicAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<class UNiagaraSystem> BasicEffect;

	virtual void SpawnBasicEffect();

// Enemy Stat
public:
	virtual float GetMaxHp() override { return MaxHp; };
	virtual float GetCurrentHp() override { return CurrentHp; };

protected:
	virtual void SetHp(float NewHp) override;


	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

// Widget
protected:
	UTGHpBarWidget* HpBarWidget;
	virtual void SetupCharacterWidget(class UTGUserWidget* InUserWidget) override;
};

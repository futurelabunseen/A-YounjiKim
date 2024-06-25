// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterNonPlayer.h"
#include "TGCharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API ATGCharacterBoss : public ATGCharacterNonPlayer
{
	GENERATED_BODY()
	
public:
	ATGCharacterBoss();

	void EnableAttack();
	void DisableAttack();
	bool isAttackable() { return canAttack; };
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	float GetAttackPower() { return AttackPower; };
	void SetAttackPower(float newPower) { this->AttackPower = newPower; };
	virtual void AttackByAI(EAttackType AttackType, TObjectPtr<class AActor> Target) override;
	virtual void EndAttack(UAnimMontage* TargetMontage, bool IsProperlyEnded) override;
	void GroundAttack(TObjectPtr<class AActor> Target);
	virtual void BasicAttack() override;
	void Lure(TObjectPtr<class AActor> Target);

protected:
	virtual void SetDead() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<class UNiagaraSystem> GroundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> GroundAttackMontage;

	virtual void SpawnBasicEffect() override;

private:
	float AttackPower = 10.f;
	bool canAttack = true;
	int GroundAttackCount = 1;
	int LureCount = 1;
};

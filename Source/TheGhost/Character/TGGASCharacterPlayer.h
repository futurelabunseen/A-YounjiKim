// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "TGGASCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API ATGGASCharacterPlayer : public ATGCharacterPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ATGGASCharacterPlayer();
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// ���ǵ� �� ȣ��Ǵ� �Լ� - ��Ƽ) ���������� ȣ��Ǵ� ���� (OnLapPlayerState)
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// GAS�� ���õ� Input�� ���ν����ִ� �Լ�
	void SetupGASInputComponent();

	// 
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities; // ������ �ο��� �����Ƽ ���
};

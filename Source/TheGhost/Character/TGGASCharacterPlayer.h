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
	
	// 빙의될 때 호출되는 함수 - 멀티) 서버에서만 호출되는 형태 (OnLapPlayerState)
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// GAS에 관련된 Input을 매핑시켜주는 함수
	void SetupGASInputComponent();

	// 
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities; // 폰에게 부여할 어빌리티 목록
};

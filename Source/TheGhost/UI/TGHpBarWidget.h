// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TGUserWidget.h"
#include "TGHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API UTGHpBarWidget : public UTGUserWidget
{
	GENERATED_BODY()
	
public:
	UTGHpBarWidget(const FObjectInitializer& ObjectInitializer);
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override; // HpProgressBar �����͸� �������� ����.

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};

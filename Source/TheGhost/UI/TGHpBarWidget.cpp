// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TGHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/TGCharacterWidgetInterface.h"

UTGHpBarWidget::UTGHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UTGHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}

void UTGHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Pb_HpBar")));
	ensure(HpProgressBar); // 위젯을 사용하기 위해서는 ProgressBar가 반드시 있어야 하기 때문에 체크

	ITGCharacterWidgetInterface* CharacterWidget = Cast<ITGCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TGWidgetComponent.h"
#include "TGUserWidget.h"

void UTGWidgetComponent::InitWidget()
{
	Super::InitWidget(); // 해당 함수가 호출될 때에는 위젯에 대한 인스턴스가 생성된 직후이다.

	UTGUserWidget* TGUserWidget = Cast<UTGUserWidget>(GetWidget());
	if (TGUserWidget)
	{
		TGUserWidget->SetOwningActor(GetOwner());
		/*UE_LOG(LogTemp, Log, TEXT("test"));*/
	}
}

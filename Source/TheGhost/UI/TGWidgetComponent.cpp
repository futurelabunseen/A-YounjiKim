// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TGWidgetComponent.h"
#include "TGUserWidget.h"

void UTGWidgetComponent::InitWidget()
{
	Super::InitWidget(); // �ش� �Լ��� ȣ��� ������ ������ ���� �ν��Ͻ��� ������ �����̴�.

	UTGUserWidget* TGUserWidget = Cast<UTGUserWidget>(GetWidget());
	if (TGUserWidget)
	{
		TGUserWidget->SetOwningActor(GetOwner());
		/*UE_LOG(LogTemp, Log, TEXT("test"));*/
	}
}

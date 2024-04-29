// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TGComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API UTGComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UTGComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix; // ��Ÿ�� ���� �̸� ���� (� ��Ÿ�� ���ǵ��� ����� ������)

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount; // ��� �޺� �׼��� �ִ��� ����

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate; // �������� ���� ��� �ӵ� (���� 30������)

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount; // ���� �Է� ������ ���� (�� �����ӿ� �Է��� ���Դ��� �ȵ��Դ��� üũ�� ���ΰ�)
};

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
	FString MontageSectionNamePrefix; // 몽타주 섹션 이름 지정 (어떤 몽타주 섹션들을 재생할 것인지)

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount; // 몇개의 콤보 액션이 있는지 지정

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate; // 프레임의 기준 재생 속도 (보통 30프레임)

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount; // 사전 입력 프레임 설정 (몇 프레임에 입력이 들어왔는지 안들어왔는지 체크할 것인가)
};

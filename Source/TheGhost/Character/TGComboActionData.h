// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TGComboActionData.generated.h"

USTRUCT()
struct FComboAction {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Combo")
	FName ActionName; // 액션 이름

	UPROPERTY(EditAnywhere, Category = "Combo")
	FName InputKey; // 이 액션에 할당된 입력 키

	UPROPERTY(EditAnywhere, Category = "Combo")
	int32 ActionAmount; // 콤보 시 증가시킬 데미지 양

	UPROPERTY(EditAnywhere, Category = "Combo")
	FName PrevValidInput; // 이 액션을 실행하기 위해 유효한 이전 입력 키
};

UCLASS()
class THEGHOST_API UTGComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UTGComboActionData();

	UPROPERTY(EditAnywhere, Category = "Combo")
	TArray<FComboAction> ComboActions;

private:

	UPROPERTY(EditAnywhere, Category = "Combo")
	uint8 MaxComboCount; // 몇개의 콤보 액션이 있는지 지정

	//UPROPERTY(EditAnywhere, Category = Name)
	//FString MontageSectionNamePrefix; // 몽타주 섹션 이름 지정 (어떤 몽타주 섹션들을 재생할 것인지)

	//UPROPERTY(EditAnywhere, Category = Name)
	//float FrameRate; // 프레임의 기준 재생 속도 (보통 30프레임)

	//UPROPERTY(EditAnywhere, Category = ComboData)
	//TArray<float> EffectiveFrameCount; // 사전 입력 프레임 설정 (몇 프레임에 입력이 들어왔는지 안들어왔는지 체크할 것인가)
};

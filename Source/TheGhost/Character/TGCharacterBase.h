// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TGAnimationAttackInterface.h"
#include "TGCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class THEGHOST_API ATGCharacterBase : public ACharacter, public ITGAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATGCharacterBase();
protected:
	// 
	virtual void SetCharacterControlData(const class UTGCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UTGCharacterControlData*> CharacterControlManager;

	// 몽타주 섹션
	// 만약 나중에 만들 NPC도 현재의 몽타주 애니메이션을 재생하게 하고 싶다면 아래 정보를 CharacterBase 헤더에 넣을 것.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGComboActionData> ComboActionData;

	int32 CurrentCombo = 0; // 현재 진행중인 콤보 - 코드에서만 사용할 것이기 때문에 UPROPERTY 붙이지 X / 0일 때는 아직 콤보가 시작되지 않음.
	FTimerHandle ComboTimerHandle; // 타이머
	bool HasNextComboCommand = false; // 발동된 타이머 이전에 입력 커맨드가 들어왔으면 true

	void ProcessComboCommand();
	void ComboActionBegin(); // 몽타주가 시작될 때 호출
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded); // 몽타주가 종료되면 호출 - 몽타주에 설정된 델리게이트를 통해 바로 호출될 수 있도록 함. UAnimMontage 클래스의 FOnMontageEnded 델리게이트를 사용하기 위한 파라미터들.
	void SetComboCheckTimer(); // 타이머를 발동시키는 함수
	void ComboCheck(); // 타이머가 발동됐을 때 입력이 들어왔는지 안들어왔는지 체크

	// 공격 판정 섹션
protected:
	virtual void AttackHitCheck() override;
};

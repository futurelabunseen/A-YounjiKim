// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterBase.h"
#include "InputActionValue.h"
#include "TGCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API ATGCharacterPlayer : public ATGCharacterBase
{
	GENERATED_BODY()
public:
	ATGCharacterPlayer();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // 아래 3가지 액션들과 Move, Look 함수를 언리얼 엔진의 입력 시스템에 매핑

protected:
	// Character Control Section
	void ChangeCharacterControl(); // 인풋매핑컨텍스트를 바꿔치기하는 함수
	void SetCharacterControl(ECharacterControlType NewCharacterControlType); // 컨트롤에 관한 모든 설정을 진행
	virtual void SetCharacterControlData(const class UTGCharacterControlData* CharacterControlData) override;

	// 아래의 매핑 컨텍스트를 할당하는 역할
	virtual void BeginPlay() override;

	// 인풋이 들어가면 BindAction 함수로 인해 실행되는 함수들
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

// 몽타주 섹션
// 만약 나중에 만들 NPC도 현재의 몽타주 애니메이션을 재생하게 하고 싶다면 아래 정보를 CharacterBase 헤더에 넣을 것.
protected:
	int32 CurrentCombo = 0; // 현재 진행중인 콤보 - 코드에서만 사용할 것이기 때문에 UPROPERTY 붙이지 X / 0일 때는 아직 콤보가 시작되지 않음.
	FName CurrentInputKey;
	FName LastInputKey = FName();
	bool isPlaying = false;

	void ProcessComboCommand(FName ActionName);
	void PlayAttackMontage(FName ActionName);
	void ResetComboAction();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded); // 몽타주가 종료되면 호출 - 몽타주에 설정된 델리게이트를 통해 바로 호출될 수 있도록 함. UAnimMontage 클래스의 FOnMontageEnded 델리게이트를 사용하기 위한 파라미터들.

	void Attack1Pressed();
	void Attack2Pressed();
	void Attack3Pressed();
	void Attack4Pressed();

	void SetDead() override;
	void PlayDeadAnimation() override;


// 카메라와 관련된 설정
protected:
	// 스프링 암 - 카메라를 지탱해주는 지지대 역할
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true")) // Meta 추가 지정자 = private으로 선언된 UE 객체들을 블루프린트에서도 접근할 수 있도록 만들어주는 특별한 지시자
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// 실제 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// 입력에 대한 설정
	// 다른 에셋으로 변경할 수 있도록 설계하기 위해 지정자를 EditAnywhere로 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction; // 액션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction; // V키로 시점 변경

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UInputAction>> AttackAction;

	ECharacterControlType CurrentCharacterControlType;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;
};

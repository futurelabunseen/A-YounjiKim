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
	//void ChangeCharacterControl();
	//void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UTGCharacterControlData* CharacterControlData) override;

	// 아래의 매핑 컨텍스트를 할당하는 역할
	virtual void BeginPlay() override;

	// 카메라와 관련된 설정
	// 스프링 암 - 카메라를 지탱해주는 지지대 역할
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true")) // Meta 추가 지정자 = private으로 선언된 UE 객체들을 블루프린트에서도 접근할 수 있도록 만들어주는 특별한 지시자
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// 실제 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// 입력에 대한 설정
	// 다른 에셋으로 변ㄱ경할 수 있도록 설계하기 위해 지정자를 EditAnywhere로 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext; // 매핑 컨텍스트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction; // 액션

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};

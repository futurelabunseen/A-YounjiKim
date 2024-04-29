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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // �Ʒ� 3���� �׼ǵ�� Move, Look �Լ��� �𸮾� ������ �Է� �ý��ۿ� ����

protected:
	// Character Control Section
	void ChangeCharacterControl(); // ��ǲ�������ؽ�Ʈ�� �ٲ�ġ���ϴ� �Լ�
	void SetCharacterControl(ECharacterControlType NewCharacterControlType); // ��Ʈ�ѿ� ���� ��� ������ ����
	virtual void SetCharacterControlData(const class UTGCharacterControlData* CharacterControlData) override;

	// �Ʒ��� ���� ���ؽ�Ʈ�� �Ҵ��ϴ� ����
	virtual void BeginPlay() override;

	// ��ǲ�� ���� BindAction �Լ��� ���� ����Ǵ� �Լ���
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	void Attack();

protected:
	// ī�޶�� ���õ� ����
	// ������ �� - ī�޶� �������ִ� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true")) // Meta �߰� ������ = private���� ����� UE ��ü���� �������Ʈ������ ������ �� �ֵ��� ������ִ� Ư���� ������
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// ���� ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// �Է¿� ���� ����
	// �ٸ� �������� ������ �� �ֵ��� �����ϱ� ���� �����ڸ� EditAnywhere�� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction; // �׼�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction; // VŰ�� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	ECharacterControlType CurrentCharacterControlType;
};

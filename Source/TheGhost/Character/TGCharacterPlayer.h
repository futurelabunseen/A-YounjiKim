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

// ��Ÿ�� ����
// ���� ���߿� ���� NPC�� ������ ��Ÿ�� �ִϸ��̼��� ����ϰ� �ϰ� �ʹٸ� �Ʒ� ������ CharacterBase ����� ���� ��.
protected:
	int32 CurrentCombo = 0; // ���� �������� �޺� - �ڵ忡���� ����� ���̱� ������ UPROPERTY ������ X / 0�� ���� ���� �޺��� ���۵��� ����.
	FName CurrentInputKey;
	FName LastInputKey = FName();
	bool isPlaying = false;

	void ProcessComboCommand(FName ActionName);
	void PlayAttackMontage(FName ActionName);
	void ResetComboAction();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded); // ��Ÿ�ְ� ����Ǹ� ȣ�� - ��Ÿ�ֿ� ������ ��������Ʈ�� ���� �ٷ� ȣ��� �� �ֵ��� ��. UAnimMontage Ŭ������ FOnMontageEnded ��������Ʈ�� ����ϱ� ���� �Ķ���͵�.

	void Attack1Pressed();
	void Attack2Pressed();
	void Attack3Pressed();
	void Attack4Pressed();

	void SetDead() override;
	void PlayDeadAnimation() override;


// ī�޶�� ���õ� ����
protected:
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

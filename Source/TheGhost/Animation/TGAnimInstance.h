// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API UTGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTGAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character) // ��ӹ��� �������Ʈ������ ���� ���� ����
	TObjectPtr<class ACharacter> Owner; // �� AnimInstance�� �����ϰ� �ִ� ĳ������ ������ ��� Owner

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement; // ĳ���� �����Ʈ ������Ʈ�� ���� ��ü �����͸� ��� ����

	// ���� ���� ���� ������ - ��� ����Ǳ� ������ EditAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity; // �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed; // �������� ĳ������ �̵� �ӷ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1; // ���� Idle ��������? 1�̸� Idle

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold; // Idle ���¸� üũ�ϱ� ����, '�����δ�'�� �Ǵ��� �Ӱ谪. �̵��ӵ��� Threshold���� Ŀ���� �����̴� ���·� �Ǵ�.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1; // ���� �������� �ִ� ��������?

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1; // ���� �����ϰ� �ִ� ��������?

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold; // ���� ������ �Ǵ��ϴ� ���ذ�. 
};

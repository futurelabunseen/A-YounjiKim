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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character) // 상속받은 블루프린트에서는 읽을 수만 있음
	TObjectPtr<class ACharacter> Owner; // 이 AnimInstance를 소유하고 있는 캐릭터의 정보를 담는 Owner

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement; // 캐릭터 무브먼트 컴포넌트에 대한 객체 포인터를 담는 변수

	// 여러 값에 대한 정보들 - 계속 변경되기 때문에 EditAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity; // 속도

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed; // 땅에서의 캐릭터의 이동 속력

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1; // 현재 Idle 상태인지? 1이면 Idle

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold; // Idle 상태를 체크하기 위해, '움직인다'란 판단의 임계값. 이동속도가 Threshold보다 커지면 움직이는 상태로 판단.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1; // 현재 떨어지고 있는 상태인지?

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1; // 현재 점프하고 있는 상태인지?

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold; // 점프 중임을 판단하는 기준값. 
};

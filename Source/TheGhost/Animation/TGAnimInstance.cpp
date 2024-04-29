// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TGAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UTGAnimInstance::UTGAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UTGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	/* Owner ��ü �ʱ�ȭ */
	Owner = Cast<ACharacter>(GetOwningActor()); // �ش� AnimInstance�� �����ϰ� �ִ� Actor�� ��ȯ�ϴµ�, �̸� Character�� ����ȯ.
	if (Owner)
	{
		/* Movement ��ü �ʱ�ȭ */
		Movement = Owner->GetCharacterMovement();
	}
}

void UTGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �����Ӹ��� Movement ��ü�� ���� ���ϴ� ���� ������
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D(); // z������ �ӷ��� ������ xy ����� �ӷ�
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
	}
}

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

	/* Owner 객체 초기화 */
	Owner = Cast<ACharacter>(GetOwningActor()); // 해당 AnimInstance를 소유하고 있는 Actor를 반환하는데, 이를 Character로 형변환.
	if (Owner)
	{
		/* Movement 객체 초기화 */
		Movement = Owner->GetCharacterMovement();
	}
}

void UTGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 프레임마다 Movement 객체를 통해 원하는 값들 얻어오기
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D(); // z방향의 속력을 제외한 xy 평면의 속력
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
	}
}

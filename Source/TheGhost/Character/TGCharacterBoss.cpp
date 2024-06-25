// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterBoss.h"
#include "AI/TGBossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/TGCharacterStatComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "UI/TGWidgetComponent.h"
#include "UI/TGHpBarWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"

ATGCharacterBoss::ATGCharacterBoss()
{
	MaxHp = 100.f;
	CurrentHp = MaxHp;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/TheGhost/Animation/AM_Enemy_GroundAttack.AM_Enemy_GroundAttack'"));
	if (GroundAttackMontageRef.Object)
	{
		GroundAttackMontage = GroundAttackMontageRef.Object;
	}

	// Detail 창의 AI 부분에 값 지정
	AIControllerClass = ATGBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATGCharacterBoss::EnableAttack()
{
	this->canAttack = true;
}

void ATGCharacterBoss::DisableAttack()
{
	this->canAttack = false;
}

float ATGCharacterBoss::GetAIDetectRange()
{
	return 2000.0f;
}

float ATGCharacterBoss::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

void ATGCharacterBoss::AttackByAI(EAttackType AttackType, TObjectPtr<class AActor> Target)
{
	if (Target && isAttackable())
	{
		// 보스가 플레이어와 직선 상에 있고, 플레이어를 바라보도록 방향 조절
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		FRotator NewControlRotation = Direction.Rotation();
		NewControlRotation.Pitch = 0.0f;
		NewControlRotation.Roll = 0.0f;
		SetActorRotation(NewControlRotation);

		if (AttackType == EAttackType::Basic)
		{
			BasicAttack();
		}
		else if (AttackType == EAttackType::Ground)
		{
			GroundAttack(Target);
		}
		else if (AttackType == EAttackType::Lure)
		{
			Lure(Target);
		}
	}
}

void ATGCharacterBoss::EndAttack(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	Super::EndAttack(TargetMontage, IsProperlyEnded);
}

void ATGCharacterBoss::GroundAttack(TObjectPtr<class AActor> Target)
{
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 공격 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(GroundAttackMontage, AttackSpeedRate);

	if (Target && GroundEffect && GroundAttackCount > 0)
	{
		FVector TargetLocation = Target->GetActorLocation();

		// 이펙트가 보스와 캐릭터 사이, 캐릭터의 앞쪽에서 생성되도록 조정
		float OffsetDistance = -500.0f;
		FVector TargetDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
		FVector SpawnLocation = TargetLocation + TargetDirection * OffsetDistance;
		FRotator SpawnRotation = TargetDirection.Rotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundEffect, SpawnLocation, SpawnRotation);

		// Damage 처리
		FDamageEvent DamageEvent; // 들어오는 데미지 종류 지정
		Target->TakeDamage(30, DamageEvent, GetController(), this);
	}

	GroundAttackCount--;
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ATGCharacterBoss::EndAttack);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, GroundAttackMontage);
}

void ATGCharacterBoss::BasicAttack()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(BasicAttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ATGCharacterBoss::EndAttack);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BasicAttackMontage);

	if (BasicEffect)
	{
		float MontageLength = BasicAttackMontage->GetPlayLength();
		float SpawnTime = MontageLength * 0.3f; // 몽타주의 특정 시점에 이펙트가 나타나게 지정
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATGCharacterBoss::SpawnBasicEffect, SpawnTime, false);
	}
}

void ATGCharacterBoss::SpawnBasicEffect()
{
	FVector BossLocation = GetActorLocation();
	FRotator BossRotation = GetActorRotation();

	// 보스가 앞을 바라보고 있는 방향 벡터
	FVector ForwardVector = GetActorForwardVector();
	// 이펙트가 스폰될 위치 - 앞쪽
	FVector SpawnLocation = BossLocation + ForwardVector * 100.0f;

	// 몽타주에 맞게 이펙트 회전
	FRotator SpawnRotation = BossRotation;
	SpawnRotation.Roll += 100.f;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BasicEffect, SpawnLocation, SpawnRotation);
}

void ATGCharacterBoss::Lure(TObjectPtr<class AActor> Target)
{
	FVector BossLocation = GetActorLocation();
	FVector LureLocation = BossLocation + FVector(100.0f, 0.0f, 0.0f);  // 보스 위치 근처의 특정 지점

	// 플레이어 이동 설정
	ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;  // 이동 속도 설정
	PlayerCharacter->SetActorLocation(LureLocation);
}

void ATGCharacterBoss::SetDead()
{
	Super::Super::SetDead();
	PlayDeadAnimation(); // 죽는 몽타주 재생

	ATGBossAIController* AIController = GetController<ATGBossAIController>();
	AIController->StopAI();

	FTimerHandle DeadTimerHandle; // 일정 시간이 지난 후 사라지게 하기 위한 타이머
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda( // 람다 함수를 이용하여 Timer Delegate에 멤버 함수를 부착
		[&]()
		{
			Destroy(); // 액터 제공 함수
		}
	), DeadEventDelayTime, false);
}

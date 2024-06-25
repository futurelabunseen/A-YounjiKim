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

	// Detail â�� AI �κп� �� ����
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
		// ������ �÷��̾�� ���� �� �ְ�, �÷��̾ �ٶ󺸵��� ���� ����
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

	// ���� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(GroundAttackMontage, AttackSpeedRate);

	if (Target && GroundEffect && GroundAttackCount > 0)
	{
		FVector TargetLocation = Target->GetActorLocation();

		// ����Ʈ�� ������ ĳ���� ����, ĳ������ ���ʿ��� �����ǵ��� ����
		float OffsetDistance = -500.0f;
		FVector TargetDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
		FVector SpawnLocation = TargetLocation + TargetDirection * OffsetDistance;
		FRotator SpawnRotation = TargetDirection.Rotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundEffect, SpawnLocation, SpawnRotation);

		// Damage ó��
		FDamageEvent DamageEvent; // ������ ������ ���� ����
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
		float SpawnTime = MontageLength * 0.3f; // ��Ÿ���� Ư�� ������ ����Ʈ�� ��Ÿ���� ����
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATGCharacterBoss::SpawnBasicEffect, SpawnTime, false);
	}
}

void ATGCharacterBoss::SpawnBasicEffect()
{
	FVector BossLocation = GetActorLocation();
	FRotator BossRotation = GetActorRotation();

	// ������ ���� �ٶ󺸰� �ִ� ���� ����
	FVector ForwardVector = GetActorForwardVector();
	// ����Ʈ�� ������ ��ġ - ����
	FVector SpawnLocation = BossLocation + ForwardVector * 100.0f;

	// ��Ÿ�ֿ� �°� ����Ʈ ȸ��
	FRotator SpawnRotation = BossRotation;
	SpawnRotation.Roll += 100.f;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BasicEffect, SpawnLocation, SpawnRotation);
}

void ATGCharacterBoss::Lure(TObjectPtr<class AActor> Target)
{
	FVector BossLocation = GetActorLocation();
	FVector LureLocation = BossLocation + FVector(100.0f, 0.0f, 0.0f);  // ���� ��ġ ��ó�� Ư�� ����

	// �÷��̾� �̵� ����
	ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;  // �̵� �ӵ� ����
	PlayerCharacter->SetActorLocation(LureLocation);
}

void ATGCharacterBoss::SetDead()
{
	Super::Super::SetDead();
	PlayDeadAnimation(); // �״� ��Ÿ�� ���

	ATGBossAIController* AIController = GetController<ATGBossAIController>();
	AIController->StopAI();

	FTimerHandle DeadTimerHandle; // ���� �ð��� ���� �� ������� �ϱ� ���� Ÿ�̸�
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda( // ���� �Լ��� �̿��Ͽ� Timer Delegate�� ��� �Լ��� ����
		[&]()
		{
			Destroy(); // ���� ���� �Լ�
		}
	), DeadEventDelayTime, false);
}

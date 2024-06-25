// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Physics/TGCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/TGAIController.h"
#include "CharacterStat/TGCharacterStatComponent.h"
#include "UI/TGWidgetComponent.h"
#include "UI/TGHpBarWidget.h"
#include "NiagaraFunctionLibrary.h"

ATGCharacterNonPlayer::ATGCharacterNonPlayer()
{
	//GetMesh()->SetHiddenInGame(true);
	MaxHp = 50.f;
	CurrentHp = MaxHp;

	// Root Component에 대한 Capsule
	//GetCapsuleComponent()->InitCapsuleSize(10.f, 42.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_TGCAPSULE);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/TheGhost/Animation/ABP_TG_Enemy.ABP_TG_Enemy_C"));
	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/TheGhost/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// 스켈레탈 메쉬 컴포넌트에 실제 에셋을 부착해주기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.Skeleton'/Game/Animations_Creature/Mesh/SK_Mannequin_Skeleton.SK_Mannequin_Skeleton'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/TheGhost/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BasicAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/TheGhost/Animation/AM_Enemy_BasicSitAttack.AM_Enemy_BasicSitAttack'"));
	if (BasicAttackMontageRef.Object)
	{
		BasicAttackMontage = BasicAttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/TheGhost/Animation/AM_Enemy_Dead.AM_Enemy_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// Detail 창의 AI 부분에 값 지정
	AIControllerClass = ATGAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATGCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	PlayDeadAnimation(); // 죽는 몽타주 재생
	ATGAIController* AIController = GetController<ATGAIController>();
	AIController->StopAI();

	FTimerHandle DeadTimerHandle; // 일정 시간이 지난 후 사라지게 하기 위한 타이머
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda( // 람다 함수를 이용하여 Timer Delegate에 멤버 함수를 부착
		[&]()
		{
			Destroy(); // 액터 제공 함수
		}
	), DeadEventDelayTime, false); // 부모 클래스에 있는 DelayTime을 설정해주고 loop는 X(false)
}

void ATGCharacterNonPlayer::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); // AnimInstance 가져오기
	AnimInstance->StopAllMontages(0.0f); // 진행되던 다른 모션 중지
	AnimInstance->Montage_Play(DeadMontage, 1.0f); // 죽는 몽타주를 기본 속도로 실행
}

void ATGCharacterNonPlayer::BasicAttack()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(BasicAttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ATGCharacterNonPlayer::EndAttack);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BasicAttackMontage);

	if (BasicEffect)
	{
		float MontageLength = BasicAttackMontage->GetPlayLength();
		float SpawnTime = MontageLength * 0.3f; // Midpoint of the animation
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATGCharacterNonPlayer::SpawnBasicEffect, SpawnTime, false);
	}
}

float ATGCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float ATGCharacterNonPlayer::GetAIDetectRange()
{
	return 700.0f;
}

float ATGCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float ATGCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void ATGCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ATGCharacterNonPlayer::AttackByAI(EAttackType AttackType, TObjectPtr<class AActor> Target)
{
	if (AttackType == EAttackType::Basic)
	{
		BasicAttack();
	}
}

void ATGCharacterNonPlayer::EndAttack(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

void ATGCharacterNonPlayer::SpawnBasicEffect()
{
	FVector MonsterLocation = GetActorLocation();
	FRotator MonsterRotation = GetActorRotation();

	// Calculate the spawn location in front of the boss
	FVector ForwardVector = GetActorForwardVector();
	FVector SpawnLocation = MonsterLocation + ForwardVector * 100.0f; // Adjust the distance as needed

	// Use the boss's rotation for the effect
	FRotator SpawnRotation = MonsterRotation;
	SpawnRotation.Roll += 100.f;

	// Spawn the Niagara effect at the calculated location and rotation
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BasicEffect, SpawnLocation, SpawnRotation);
}

void ATGCharacterNonPlayer::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	HpBarWidget->UpdateHpBar(CurrentHp);
}

void ATGCharacterNonPlayer::SetupCharacterWidget(UTGUserWidget* InUserWidget)
{
	HpBarWidget = Cast<UTGHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(MaxHp);
		HpBarWidget->UpdateHpBar(CurrentHp); // CurrentHP로 위젯 업데이트
	}
}

float ATGCharacterNonPlayer::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		SetDead();
	}

	return ActualDamage;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Physics/TGCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TGCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/TGCharacterStatComponent.h"
#include "UI/TGWidgetComponent.h"
#include "UI/TGHpBarWidget.h"
#include "TGCharacterPlayer.h"
#include "TGCharacterNonPlayer.h"
#include "TGCharacterBoss.h"

// Sets default values
ATGCharacterBase::ATGCharacterBase()
{
	// Pawn - 카메라 회전
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule Collider에 관한 설정은 각 파일에서(CharacterPlayer/NonCharacterPlayer)

	// 움직임 - jump에 대한 크기, 이동속도
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh - 캐릭터의 움직임에 따라 모델링 파일을 진행 방향에 맞춰 상대 위치를 조정해주어야 함.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // 이후 강좌
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UTGCharacterControlData> ShoulderDataRef(TEXT("/Script/TheGhost.TGCharacterControlData'/Game/TheGhost/CharactorControl/TGC_Shoulder.TGC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTGCharacterControlData> QuaterDataRef(TEXT("/Script/TheGhost.TGCharacterControlData'/Game/TheGhost/CharactorControl/TGC_Quarter.TGC_Quarter'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	// Stat 컴포넌트 초기화
	Stat = CreateDefaultSubobject<UTGCharacterStatComponent>(TEXT("Stat"));

	// Widget Component 
	HpBar = CreateDefaultSubobject<UTGWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh()); // 메쉬 정보 부착
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f)); // 위치 - 머리 꼭대기
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/TheGhost/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen); // 위젯스페이스는 스크린(=2D)에 띄울 것. World = 3D
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f)); // 위젯이 담겨 있는 드로잉 패널의 크기
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision); // UI에는 충돌 기능 없애기
	}
}

void ATGCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Stat->OnHpZero.AddUObject(this, &ATGCharacterBase::SetDead);
}

void ATGCharacterBase::SetCharacterControlData(const UTGCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement 설정과 관련된 데이터 세팅
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}


void ATGCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this); // "Attack" 태그로 콜리전 분석

	float AttackRange = Stat->GetTotalStat().AttackRange;
	float AttackDamage = Stat->GetTotalStat().Attack;
	if (ATGCharacterBoss* Boss = Cast<ATGCharacterBoss>(this))
	{
		AttackDamage = Boss->GetAttackPower();
	}
	else if (ATGCharacterNonPlayer* PlayerCharacter = Cast<ATGCharacterNonPlayer>(this))
	{
		AttackDamage = 10.f;
	}
	const float AttackRadius = Stat->GetAttackRadius();
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// param : (결과값을 받아오는 HitResult, 투사 시작 지점, 투사 끝 지점, _ , TraceChannel, 구체의 영역 지정(구체 반지름이 50cm), 
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_TGACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		FDamageEvent DamageEvent; // 들어오는 데미지 종류 지정
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

//#if ENABLE_DRAW_DEBUG
//
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsuleHalfHeight = AttackRange * 0.5f;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red; // 충돌하면 초록, 충돌 안한 상태면 빨강.
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
//
//#endif
}

float ATGCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // 들어온 데미지 양, 데미지 이벤트, 가해자, 가해자가 사용한 무기 또는 빙의한 폰 정보

	// 방어력이 있으면 DamageAmount의 양을 줄인다.

	// 플레이어 캐릭터일 경우
	if (ATGCharacterPlayer* PlayerCharacter = Cast<ATGCharacterPlayer>(this))
	{
		Stat->ApplyDamage(DamageAmount);
	}
	else if (ATGCharacterNonPlayer* NonPlayer = Cast<ATGCharacterNonPlayer>(this))
	{
		NonPlayer->ApplyDamage(DamageAmount);
	}
	return DamageAmount; // 최종으로 액터가 받은 데미지의 양
}

void ATGCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None); // 캐릭터가 더 이상 움직일 수 없도록 함.
	SetActorEnableCollision(false); // 충돌에 대한 기능을 꺼준다. (죽은 캐릭터는 다른 캐릭터의 이동을 방해하지 않도록)
	HpBar->SetHiddenInGame(true);
}

void ATGCharacterBase::PlayDeadAnimation()
{
}

void ATGCharacterBase::SetupCharacterWidget(UTGUserWidget* InUserWidget)
{
	UTGHpBarWidget* HpBarWidget = Cast<UTGHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp()); // CurrentHP로 위젯 업데이트
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UTGHpBarWidget::UpdateHpBar); // 
	}
}

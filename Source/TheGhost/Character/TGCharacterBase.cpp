// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TGCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "TGComboActionData.h"
#include "Components/CapsuleComponent.h"
#include "Physics/TGCollision.h"
#include "Engine/DamageEvents.h"

// Sets default values
ATGCharacterBase::ATGCharacterBase()
{
	// Pawn - 카메라 회전
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Root Component에 대한 Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_TGCAPSULE);

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

	// 스켈레탈 메쉬 컴포넌트에 실제 에셋을 부착해주기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/TheGhost/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/TheGhost/Animation/ABP_TG_Character.ABP_TG_Character_C"));
	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/TheGhost/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/TheGhost/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTGComboActionData> ComboActionDataRef(TEXT("/Script/TheGhost.TGComboActionData'/Game/TheGhost/CharacterAction/TGA_Combo_Attack.TGA_Combo_Attack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/TheGhost/Animation/AM_CharacterDead.AM_CharacterDead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
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

void ATGCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0) // 아직 몽타주 재생이 시작되지 은 경우
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid()) // 타이머가 설정이 안되어 있을 경우 ComboCommand가 눌렸을 때
	{                                // 1. 이미 타이머가 발동이 되었는데 시기를 놓쳐서 타이머가 끝난 경우  2. 지정된 연속 콤보가 전부 실행되어 더 이상 콤보로 넘어갈 필요가 없는 경우
		HasNextComboCommand = false; // 콤보 끊김! (또는 콤보 종료!)
	}
	else
	{
		HasNextComboCommand = true; // 이어갈 콤보가 있음!
	}
}

void ATGCharacterBase::ComboActionBegin()
{
	// Combo가 시작되면 호출되므로 콤보 시작됨을 표시
	CurrentCombo = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None); // 캐릭터의 이동 기능 중지

	// 애니메이션 세팅
	const float AttackSpeedRate = 1.0f; // 재생속도 지정

	// 입력이 들어오면 몽타주를 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate); //  몽타주 에셋을 지정하여 해당 몽타주를 플레이. 기본 속도인 1.0으로 자동 지정.	

	// 몽타주가 종료될 때 ComboActionEnd 함수가 호출되도록 함.
	FOnMontageEnded EndDelegate; // 구조체처럼 선언하고
	EndDelegate.BindUObject(this, &ATGCharacterBase::ComboActionEnd); // 이 구조체(EndDelegate)에 내가 바인딩할 정보를 추가.
	// 왜 강의에서는 같은 클래스 내 메서드인데도 &ATGCharacterBase::ComboActionEnd라고 썼을까? (강의에서는 몽타주 관련 모든 함수를 CharacterBase 클래스에 추가함.)

	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage); // 몽타주가 종료될 때 아래 함수 호출.

	// 콤보가 시작될 때 Timer를 발동시킨다!
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ATGCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0); // 몽타주가 종료될 때에는 몽타주가 재생되고 있어야 하기 때문에 0이 나오면 에러가 발생하도록
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ATGCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1; // 현재 재생되고 있는 몽타주 섹션의 인덱스 정보 얻기 (첫번째 인덱스 섹션이 재생되고 있으면 0번째인 거임)
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex)); // 블루프린트에서 설정한 값이 들어있는 EffectiveFrameCount 배열 중, ComboIndex 값이 배열 안에 들어있는지 검사.

	const float AttackSpeedRate = 1.0f; // 공격 속도
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate; // 해당 공격 속도로 진행했을 때 소요되는 시간(프레임)
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ATGCharacterBase::ComboCheck, ComboEffectiveTime, false); // World가 Timer를 재생시키도록 한다. ComboEffectiveTime Interval마다 ComboCheck함수를 호출하게끔 하는데, 반복 실행은 false로 설정.
	}
}

void ATGCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate(); // 타이머가 발동되서 이 함수로 들어온 경우, Timer는 할 일을 다 끝냈으므로 초기화시켜준다.
	if (HasNextComboCommand) // 타이머 발동 전에 입력이 들어와서 True가 되었다면 다음 섹션으로 넘겨준다.
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount); // 현재 combo 몽타주의 다음 섹션으로 넘긴다combo+1. maxcombocount를 벗어나지 않도록 clamp(최솟값과 최대값 사이로 값을 제한시킴)
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo); // 다음 섹션의 이름 정보를 가져오기
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage); // ComboActionMontage의 NextSection으로 바로 점프한다.
		SetComboCheckTimer(); // 다음 콤보를 위해 타이머를 걸어준다.
		HasNextComboCommand = false; // 입력값 초기화
	}
}

void ATGCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// param : (결과값을 받아오는 HitResult, 투사 시작 지점, 투사 끝 지점, _ , TraceChannel, 구체의 영역 지정(구체 반지름이 50cm), 
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_TGACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		FDamageEvent DamageEvent; // 들어오는 데미지 종류 지정
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red; // 충돌하면 초록, 충돌 안한 상태면 빨강.

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float ATGCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // 들어온 데미지 양, 데미지 이벤트, 가해자, 가해자가 사용한 무기 또는 빙의한 폰 정보

	// 방어력이 있으면 DamageAmount의 양을 줄인다.

	SetDead(); // 공격을 받으면 바로 죽음.
	return DamageAmount; // 최종으로 액터가 받은 데미지의 양
}

void ATGCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None); // 캐릭터가 더 이상 움직일 수 없도록 함.
	PlayDeadAnimation(); // 죽는 몽타주 재생
	SetActorEnableCollision(false); // 충돌에 대한 기능을 꺼준다. (죽은 캐릭터는 다른 캐릭터의 이동을 방해하지 않도록)
}

void ATGCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); // AnimInstance 가져오기
	AnimInstance->StopAllMontages(0.0f); // 진행되던 다른 모션 중지
	AnimInstance->Montage_Play(DeadMontage, 1.0f); // 죽는 몽타주를 기본 속도로 실행
}

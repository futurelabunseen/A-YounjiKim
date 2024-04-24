// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TGCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "TGComboActionData.h"

ATGCharacterPlayer::ATGCharacterPlayer()
{
	// Camera에 관련된 오브젝트 생성과 기본 값 추가
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // 스프링 암 생성
	CameraBoom->SetupAttachment(RootComponent); // 루트 컴포넌트에 부착
	CameraBoom->TargetArmLength = 400.0f; // 4m 길이 설정
	CameraBoom->bUsePawnControlRotation = true; // 카메라 회전 설정

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); // 카메라 컴포넌트 생성
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 스프링 암에 부착 - Socket이라고 하는 이름지시자를 쓰면 스프링 암 끝에 자동으로 부착됨
	FollowCamera->bUsePawnControlRotation = false; // 카메라 회전 설정

	// Input에 관련된
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Quater;
}

void ATGCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
}

void ATGCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::Attack);
}

void ATGCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void ATGCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UTGCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings(); // 기존의 모든 인풋 매핑 컨텍스트 제거
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext; // 컨트롤 데이터에 있는 인풋 매핑 컨텍스트
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0); // 새로운 인풋 매핑 컨텍스트 추가
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void ATGCharacterPlayer::SetCharacterControlData(const UTGCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ATGCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ATGCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	// 인수로 Input값을 받아서
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ATGCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>(); // 현재 무브먼트 벡터 받아옴

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f; // 벡터 크기 1로 조정
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f); // modifier를 사용해 벡터의 X, Y 조정
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator()); // 현재 ControlRotation을 Forward 방향을 사용하여 지정하면 무브먼트 컴포넌트에서 설정한 옵션에 의해 캐릭터가 자동으로 이동하는 방향을 향해 회전한다.
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void ATGCharacterPlayer::Attack()
{
	ProcessComboCommand();
}

void ATGCharacterPlayer::ProcessComboCommand()
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

void ATGCharacterPlayer::ComboActionBegin()
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
	EndDelegate.BindUObject(this, &ATGCharacterPlayer::ComboActionEnd); // 이 구조체(EndDelegate)에 내가 바인딩할 정보를 추가.
	// 왜 강의에서는 같은 클래스 내 메서드인데도 &ATGCharacterBase::ComboActionEnd라고 썼을까? (강의에서는 몽타주 관련 모든 함수를 CharacterBase 클래스에 추가함.)
	
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage); // 몽타주가 종료될 때 아래 함수 호출.

	// 콤보가 시작될 때 Timer를 발동시킨다!
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ATGCharacterPlayer::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0); // 몽타주가 종료될 때에는 몽타주가 재생되고 있어야 하기 때문에 0이 나오면 에러가 발생하도록
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ATGCharacterPlayer::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1; // 현재 재생되고 있는 몽타주 섹션의 인덱스 정보 얻기 (첫번째 인덱스 섹션이 재생되고 있으면 0번째인 거임)
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex)); // 블루프린트에서 설정한 값이 들어있는 EffectiveFrameCount 배열 중, ComboIndex 값이 배열 안에 들어있는지 검사.

	const float AttackSpeedRate = 1.0f; // 공격 속도
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate; // 해당 공격 속도로 진행했을 때 소요되는 시간(프레임)
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ATGCharacterPlayer::ComboCheck, ComboEffectiveTime, false); // World가 Timer를 재생시키도록 한다. ComboEffectiveTime Interval마다 ComboCheck함수를 호출하게끔 하는데, 반복 실행은 false로 설정.
	}
}

void ATGCharacterPlayer::ComboCheck()
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

void ATGCharacterPlayer::AttackHitCheck()
{

}

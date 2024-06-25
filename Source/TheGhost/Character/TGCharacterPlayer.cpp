// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Physics/TGCollision.h"
#include "TGCharacterControlData.h"
#include "TGComboActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/TGCharacterStatComponent.h"
#include "Player/TGPlayerController.h"

ATGCharacterPlayer::ATGCharacterPlayer()
{
	// Camera�� ���õ� ������Ʈ ������ �⺻ �� �߰�
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // ������ �� ����
	CameraBoom->SetupAttachment(RootComponent); // ��Ʈ ������Ʈ�� ����
	CameraBoom->TargetArmLength = 400.0f; // 4m ���� ����
	CameraBoom->bUsePawnControlRotation = true; // ī�޶� ȸ�� ����

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); // ī�޶� ������Ʈ ����
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // ������ �Ͽ� ���� - Socket�̶�� �ϴ� �̸������ڸ� ���� ������ �� ���� �ڵ����� ������
	FollowCamera->bUsePawnControlRotation = false; // ī�޶� ȸ�� ����

	// Root Component�� ���� Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_TGCAPSULE);

	// Input�� ���õ�
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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttack1Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_Attack1.IA_Attack1'"));
	if (nullptr != InputActionAttack1Ref.Object)
	{
		AttackAction.Add(InputActionAttack1Ref.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttack2Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_Attack2.IA_Attack2'"));
	if (nullptr != InputActionAttack2Ref.Object)
	{
		AttackAction.Add(InputActionAttack2Ref.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttack3Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_Attack3.IA_Attack3'"));
	if (nullptr != InputActionAttack3Ref.Object)
	{
		AttackAction.Add(InputActionAttack3Ref.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttack4Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/TheGhost/Input/Actions/IA_Attack4.IA_Attack4'"));
	if (nullptr != InputActionAttack4Ref.Object)
	{
		AttackAction.Add(InputActionAttack4Ref.Object);
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/TheGhost/Animation/ABP_TG_Character.ABP_TG_Character_C"));
	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/TheGhost/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// ���̷�Ż �޽� ������Ʈ�� ���� ������ �������ֱ�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/TheGhost/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
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
	if (AttackAction.Num() > 0)
	{
		EnhancedInputComponent->BindAction(AttackAction[0], ETriggerEvent::Triggered, this, &ATGCharacterPlayer::Attack1Pressed);
		EnhancedInputComponent->BindAction(AttackAction[1], ETriggerEvent::Triggered, this, &ATGCharacterPlayer::Attack2Pressed);
		EnhancedInputComponent->BindAction(AttackAction[2], ETriggerEvent::Triggered, this, &ATGCharacterPlayer::Attack3Pressed);
		EnhancedInputComponent->BindAction(AttackAction[3], ETriggerEvent::Triggered, this, &ATGCharacterPlayer::Attack4Pressed);
	}
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
		Subsystem->ClearAllMappings(); // ������ ��� ��ǲ ���� ���ؽ�Ʈ ����
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext; // ��Ʈ�� �����Ϳ� �ִ� ��ǲ ���� ���ؽ�Ʈ
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0); // ���ο� ��ǲ ���� ���ؽ�Ʈ �߰�
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
	// �μ��� Input���� �޾Ƽ�
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ATGCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>(); // ���� �����Ʈ ���� �޾ƿ�

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f; // ���� ũ�� 1�� ����
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

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f); // modifier�� ����� ������ X, Y ����
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator()); // ���� ControlRotation�� Forward ������ ����Ͽ� �����ϸ� �����Ʈ ������Ʈ���� ������ �ɼǿ� ���� ĳ���Ͱ� �ڵ����� �̵��ϴ� ������ ���� ȸ���Ѵ�.
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void ATGCharacterPlayer::SetDead()
{
	Super::SetDead();
	DisableInput(Cast<APlayerController>(GetController()));
	PlayDeadAnimation(); // �״� ��Ÿ�� ���
}

void ATGCharacterPlayer::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); // AnimInstance ��������
	AnimInstance->Montage_Stop(0.2f);
	AnimInstance->StopAllMontages(0.0f); // ����Ǵ� �ٸ� ��� ����
	AnimInstance->Montage_Play(DeadMontage, 1.0f); // �״� ��Ÿ�ָ� �⺻ �ӵ��� ����
}

void ATGCharacterPlayer::ProcessComboCommand(FName ActionName)
{
	if (CurrentCombo < ComboActionData->ComboActions.Num()) {
		FComboAction& CurrentAction = ComboActionData->ComboActions[CurrentCombo];
		
		// �޺��� �̾ �� �ִ��� Ȯ��
		if (CurrentAction.PrevValidInput.IsEqual(LastInputKey) && CurrentAction.InputKey == CurrentInputKey) {
			// �޺� ȿ�� ���� - ���� ������ �ø���
			Stat->SetAttack(CurrentAction.ActionAmount);
			LastInputKey = CurrentInputKey;
			CurrentCombo++;
		}
		else {
			// �޺� �ߴ�
			ResetComboAction();
		}
		PlayAttackMontage(ActionName);
	}
	// �ִ� ī��Ʈ �Ѿ�� ���� ���� �� �ٷ� �޺� �̾ �� �ֵ��� if�� ���ο��� ����
	if (CurrentCombo >= ComboActionData->ComboActions.Num())
		ResetComboAction();
}

void ATGCharacterPlayer::PlayAttackMontage(FName ActionName)
{
	isPlaying = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);
	AnimInstance->Montage_JumpToSection(ActionName, ComboActionMontage);

	// ��Ÿ�ְ� ����� �� ComboActionEnd �Լ��� ȣ��ǵ��� ��.
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ATGCharacterPlayer::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
}

void ATGCharacterPlayer::ResetComboAction()
{
	CurrentCombo = 0;
	LastInputKey = FName("None");
}

void ATGCharacterPlayer::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	isPlaying = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Stat->InitializeAttack();
}

void ATGCharacterPlayer::Attack1Pressed()
{
	CurrentInputKey = FName(TEXT("1"));
	if (!isPlaying)
		ProcessComboCommand(ComboActionData->ComboActions[0].ActionName);
}

void ATGCharacterPlayer::Attack2Pressed()
{
	CurrentInputKey = FName(TEXT("2"));
	if (!isPlaying)
		ProcessComboCommand(ComboActionData->ComboActions[1].ActionName);
}

void ATGCharacterPlayer::Attack3Pressed()
{
	CurrentInputKey = FName(TEXT("3"));
	if (!isPlaying)
		ProcessComboCommand(ComboActionData->ComboActions[2].ActionName);
}

void ATGCharacterPlayer::Attack4Pressed()
{
	CurrentInputKey = FName(TEXT("4"));
	if (!isPlaying)
		ProcessComboCommand(ComboActionData->ComboActions[3].ActionName);
}

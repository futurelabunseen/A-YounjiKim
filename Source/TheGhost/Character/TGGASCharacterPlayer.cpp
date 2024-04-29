// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/TGGASPlayerState.h"
#include "EnhancedInputComponent.h"

ATGGASCharacterPlayer::ATGGASCharacterPlayer()
{
	ASC = nullptr;
	// PlayerState가 Character에게 빙의될 때 PlayerState의 ASC를 여기에 대입해주면 하나의 ASC로 사용할 수 있음.
}

UAbilitySystemComponent* ATGGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ATGGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ATGGASPlayerState* GASPS = GetPlayerState<ATGGASPlayerState>(); // playerState 가져오기.
	if (GASPS) // playerState가 null이 아니면
	{
		ASC = GASPS->GetAbilitySystemComponent(); // GAS의 AbilitySystem을 폰이 가진 ASC에 대입
		ASC->InitAbilityActorInfo(GASPS, this); // 이 상태에서 오너 액터와 아바타 액터가 결정되었기 때문에 이것들을 초기화해줌 (오너액터, 현재 폰)

		// 어빌리티에 따라 아이디 부여 - 이후 Input이 들어왔을 때 어떤 어빌리티가 들어왔는지 검출할 수 있게 함.
		int32 InputId = 0;
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			// 아이디에 대한 정보를 집어넣는 스택
			StartSpec.InputID = InputId++;
			ASC->GiveAbility(StartSpec);

			// 클라이언트에서는 호출되지 않고, 서버에서만 호출되기 때문에 두 군데에서 호출해도 문제가 발생하지 않는다.
			SetupGASInputComponent();
		}
	}
}

void ATGGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();
}

void ATGGASCharacterPlayer::SetupGASInputComponent()
{
	// GAS가 설정되어 있는지 확인(ASC 작동하는지) & InputComponent가 있는지 확인
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATGGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATGGASCharacterPlayer::GASInputReleased, 0);
		//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ATGGASCharacterPlayer::GASInputPressed);
	}
}

void ATGGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	// 사용자 입력이 들어오면 특정 어빌리티를 발동시키도록 설정
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId); // 인자 InputId에 매핑된 어빌리티의 Spec 정보를 변수에 할당
	if (Spec)
	{
		Spec->InputPressed = true; // 현재 스펙 상태를 Input이 들어온 상태라고 변경해줌.
		if (Spec->IsActive()) // 어빌리티가 이미 발동되어 있는 상태인지 판단
		{
			ASC->AbilitySpecInputPressed(*Spec); // 입력이 들어왔다는 신호를 어빌리티 스펙에 전달해줌. 
		}
		else // 어빌리티가 아직 발동되지 않은 경우
		{
			ASC->TryActivateAbility(Spec->Handle); // 스펙이 가지고 있는 핸들 값을 인자로 넘겨 발동되도록 제어해준다.
		}
	}
}

void ATGGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec); // 발동이 중지되도록 설정.
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/TGGASPlayerState.h"
#include "EnhancedInputComponent.h"

ATGGASCharacterPlayer::ATGGASCharacterPlayer()
{
	ASC = nullptr;
	// PlayerState�� Character���� ���ǵ� �� PlayerState�� ASC�� ���⿡ �������ָ� �ϳ��� ASC�� ����� �� ����.
}

UAbilitySystemComponent* ATGGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ATGGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ATGGASPlayerState* GASPS = GetPlayerState<ATGGASPlayerState>(); // playerState ��������.
	if (GASPS) // playerState�� null�� �ƴϸ�
	{
		ASC = GASPS->GetAbilitySystemComponent(); // GAS�� AbilitySystem�� ���� ���� ASC�� ����
		ASC->InitAbilityActorInfo(GASPS, this); // �� ���¿��� ���� ���Ϳ� �ƹ�Ÿ ���Ͱ� �����Ǿ��� ������ �̰͵��� �ʱ�ȭ���� (���ʾ���, ���� ��)

		// �����Ƽ�� ���� ���̵� �ο� - ���� Input�� ������ �� � �����Ƽ�� ���Դ��� ������ �� �ְ� ��.
		int32 InputId = 0;
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			// ���̵� ���� ������ ����ִ� ����
			
			ASC->GiveAbility(StartSpec);

			// Ŭ���̾�Ʈ������ ȣ����� �ʰ�, ���������� ȣ��Ǳ� ������ �� �������� ȣ���ص� ������ �߻����� �ʴ´�.
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
	// GAS�� �����Ǿ� �ִ��� Ȯ��(ASC �۵��ϴ���) & InputComponent�� �ִ��� Ȯ��
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATGGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATGGASCharacterPlayer::GASInputReleased);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ATGGASCharacterPlayer::GASInputPressed);
	}
}

void ATGGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	// �Է��� ������ �� Ư�� �����Ƽ ����

}

void ATGGASCharacterPlayer::GASInputReleased(int32 InputId)
{
}

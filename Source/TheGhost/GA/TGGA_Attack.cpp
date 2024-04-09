// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TGGA_Attack.h"
#include "Character/TGCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UTGGA_Attack::UTGGA_Attack()
{
	// �ν��Ͻ� �ɼ� - CDO ó��? *���� �� �ϳ��� �ν��Ͻ���?* ���� �� ���� �ν��Ͻ�?
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTGGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// �����Ƽ �ߵ� ���·� ���� (�� ��)
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// ��Ÿ�� ��ȯ
	ATGCharacterBase* TGCharacter = CastChecked<ATGCharacterBase>(ActorInfo->AvatarActor.Get()); // Character�� ����ȯ �ʿ�
	//UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ��Ÿ�־׼�);
}

void UTGGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
}

void UTGGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UTGGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

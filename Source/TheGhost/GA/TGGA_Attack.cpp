// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TGGA_Attack.h"
#include "Character/TGCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UTGGA_Attack::UTGGA_Attack()
{
	// 인스턴싱 옵션 - CDO 처리? *액터 당 하나의 인스턴스만?* 액터 당 여러 인스턴스?
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTGGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 어빌리티 발동 상태로 변경 (한 번)
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 몽타주 변환
	ATGCharacterBase* TGCharacter = CastChecked<ATGCharacterBase>(ActorInfo->AvatarActor.Get()); // Character로 형변환 필요
	//UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), 몽타주액션);
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

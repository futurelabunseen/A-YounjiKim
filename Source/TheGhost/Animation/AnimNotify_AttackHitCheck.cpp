// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/TGAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ITGAnimationAttackInterface* AttackPawn = Cast<ITGAnimationAttackInterface>(MeshComp->GetOwner()); // MeshComponent�� ���� �ִ� Owner(TGCharacterPlayer)�� Interface�� �����ߴ��� ĳ������ �Ǵ����� üũ���ش�.
		if (AttackPawn) // Owner�� �������̽��� �����ߴٸ� ĳ���õ� �� NULL�� �ƴϴϱ�!
		{
			AttackPawn->AttackHitCheck();
		}
	}
}

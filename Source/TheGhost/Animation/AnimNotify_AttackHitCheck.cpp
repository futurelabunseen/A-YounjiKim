// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/TGAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ITGAnimationAttackInterface* AttackPawn = Cast<ITGAnimationAttackInterface>(MeshComp->GetOwner()); // MeshComponent를 갖고 있는 Owner(TGCharacterPlayer)가 Interface를 구현했는지 캐스팅이 되는지로 체크해준다.
		if (AttackPawn) // Owner가 인터페이스를 구현했다면 캐스팅될 때 NULL이 아니니까!
		{
			AttackPawn->AttackHitCheck();
		}
	}
}

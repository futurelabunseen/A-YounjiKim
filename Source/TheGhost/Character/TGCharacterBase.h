// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TGAnimationAttackInterface.h"
#include "TGCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class THEGHOST_API ATGCharacterBase : public ACharacter, public ITGAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATGCharacterBase();
protected:
	// 
	virtual void SetCharacterControlData(const class UTGCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UTGCharacterControlData*> CharacterControlManager;

	// ��Ÿ�� ����
	// ���� ���߿� ���� NPC�� ������ ��Ÿ�� �ִϸ��̼��� ����ϰ� �ϰ� �ʹٸ� �Ʒ� ������ CharacterBase ����� ���� ��.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGComboActionData> ComboActionData;

	int32 CurrentCombo = 0; // ���� �������� �޺� - �ڵ忡���� ����� ���̱� ������ UPROPERTY ������ X / 0�� ���� ���� �޺��� ���۵��� ����.
	FTimerHandle ComboTimerHandle; // Ÿ�̸�
	bool HasNextComboCommand = false; // �ߵ��� Ÿ�̸� ������ �Է� Ŀ�ǵ尡 �������� true

	void ProcessComboCommand();
	void ComboActionBegin(); // ��Ÿ�ְ� ���۵� �� ȣ��
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded); // ��Ÿ�ְ� ����Ǹ� ȣ�� - ��Ÿ�ֿ� ������ ��������Ʈ�� ���� �ٷ� ȣ��� �� �ֵ��� ��. UAnimMontage Ŭ������ FOnMontageEnded ��������Ʈ�� ����ϱ� ���� �Ķ���͵�.
	void SetComboCheckTimer(); // Ÿ�̸Ӹ� �ߵ���Ű�� �Լ�
	void ComboCheck(); // Ÿ�̸Ӱ� �ߵ����� �� �Է��� ���Դ��� �ȵ��Դ��� üũ

	// ���� ���� ����
protected:
	virtual void AttackHitCheck() override;
};

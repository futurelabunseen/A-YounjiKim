// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TGAnimationAttackInterface.h"
#include "Interface/TGCharacterWidgetInterface.h"
#include "TGCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class THEGHOST_API ATGCharacterBase : public ACharacter, public ITGAnimationAttackInterface, public ITGCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATGCharacterBase();
	virtual void PostInitializeComponents() override;

protected:
	// 
	virtual void SetCharacterControlData(const class UTGCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UTGCharacterControlData*> CharacterControlManager;

	// ���� ���� ����
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Dead ����
protected:

	virtual void SetDead(); // �״� ���� ����� ���� �Լ�
	virtual void PlayDeadAnimation(); // Dead Animation ��� �Լ�

	float DeadEventDelayTime = 1.0f; // �״� ���� 5�� ������ �� ������� �ϱ� ���� ����

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGCharacterStatComponent> Stat;

	// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UTGUserWidget* InUserWidget) override;
};

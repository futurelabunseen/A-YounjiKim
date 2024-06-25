// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SafeAreaBox.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API ASafeAreaBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ASafeAreaBox();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;
	void DeactivateStealthEffect();

private:
	FTimerHandle StealthTimerHandle;
	float StealthDuration;
	TObjectPtr<class ATGCharacterPlayer> Player;
};

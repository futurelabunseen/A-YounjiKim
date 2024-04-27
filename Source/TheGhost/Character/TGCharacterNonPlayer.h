// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterBase.h"
#include "TGCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class THEGHOST_API ATGCharacterNonPlayer : public ATGCharacterBase
{
	GENERATED_BODY()
	
public:
	ATGCharacterNonPlayer();

protected:
	void SetDead() override;
};

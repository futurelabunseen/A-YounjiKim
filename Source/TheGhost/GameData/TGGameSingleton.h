// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TGCharacterStat.h"
#include "TGGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTGGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class THEGHOST_API UTGGameSingleton : public UObject
{
	GENERATED_BODY()
	
	public:
	UTGGameSingleton();
	static UTGGameSingleton& Get();

	// Character Stat Data Section
public:
	// 레벨에 해당하는 데이터를 스탯테이블에서 꺼냄.
	FORCEINLINE FTGCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FTGCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel; // 총 몇 개의 레벨이 있는가

private:
	TArray<FTGCharacterStat> CharacterStatTable; // FTGCharacterStat의 데이터를 테이블 형식으로 만들어서 저장.
};

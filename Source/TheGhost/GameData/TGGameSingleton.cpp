// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/TGGameSingleton.h"

DEFINE_LOG_CATEGORY(LogTGGameSingleton);

UTGGameSingleton::UTGGameSingleton()
{
	// raw테이블 에셋에 접근해서 Tarray로 변환하도록, 
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/TheGhost/GameData/TGCharacterStatTable.TGCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FTGCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UTGGameSingleton& UTGGameSingleton::Get()
{
	// TODO: insert return statement here
	UTGGameSingleton* Singleton = CastChecked< UTGGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	// Singleton이 유효하지 않은 상황
	UE_LOG(LogTGGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UTGGameSingleton>(); // 로직만을 위해 만든 코드로 실제로 사용하지는 않는 객체.
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/TGGameSingleton.h"

DEFINE_LOG_CATEGORY(LogTGGameSingleton);

UTGGameSingleton::UTGGameSingleton()
{
	// raw���̺� ���¿� �����ؼ� Tarray�� ��ȯ�ϵ���, 
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

	// Singleton�� ��ȿ���� ���� ��Ȳ
	UE_LOG(LogTGGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UTGGameSingleton>(); // �������� ���� ���� �ڵ�� ������ ��������� �ʴ� ��ü.
}

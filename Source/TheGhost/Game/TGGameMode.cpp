// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TGGameMode.h"
#include "GameFramework/Actor.h"
#include "TGGameMode.h"

ATGGameMode::ATGGameMode()
{
	// DefaultPawnClass - 
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/TheGhost/Blueprint/BP_TGCharacterPlayer.BP_TGCharacterPlayer_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	// PlayerControllerClass - PlayerController에 대한 설정
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/TheGhost.TGPlayerController"));
	if (PlayerControllerClassRef.Class) {
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

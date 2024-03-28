// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TGGameMode.h"
#include "TGGameMode.h"

ATGGameMode::ATGGameMode()
{
	// DefaultPawnClass - 
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Script/TheGhost.TGCharacterPlayer"));
	if (ThirdPersonClassRef.Class)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}
	// PlayerControllerClass - PlayerController에 대한 설정
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/TheGhost.TGPlayerController"));
	if (PlayerControllerClassRef.Class) {
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

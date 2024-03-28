// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TGPlayerController.h"

void ATGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작하자마자 뷰포트 안으로 포커스 들어가기
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

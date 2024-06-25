// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/SafeAreaBox.h"
#include "Character/TGCharacterPlayer.h"
#include "Character/TGCharacterBoss.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include <Kismet/GameplayStatics.h>

ASafeAreaBox::ASafeAreaBox()
{
    StealthDuration = 10.0f;
}

void ASafeAreaBox::BeginPlay()
{
    Super::BeginPlay();
    Player = Cast<ATGCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ASafeAreaBox::DeactivateStealthEffect()
{
    if (Player && Player->IsOverlappingActor(this))
    {
        // 플레이어가 더 이상 숨지 않을 때 보스 공격 활성화
        ATGCharacterBoss* Boss = Cast<ATGCharacterBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ATGCharacterBoss::StaticClass()));
        if (Boss)
        {
            Boss->EnableAttack();
        }
    }
}

void ASafeAreaBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (OtherActor && OtherActor->IsA(ATGCharacterPlayer::StaticClass()))
    {
        // 플레이어가 숨는 동안 보스 공격 비활성화
        ATGCharacterBoss* Boss = Cast<ATGCharacterBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ATGCharacterBoss::StaticClass()));
        if (Boss)
        {
            Boss->DisableAttack();
        }
        GetWorld()->GetTimerManager().SetTimer(StealthTimerHandle, this, &ASafeAreaBox::DeactivateStealthEffect, StealthDuration, false);
    }
}

void ASafeAreaBox::NotifyActorEndOverlap(AActor* OtherActor)
{
    // 플레이어가 숨는 지역에서 나왔을 때
    Super::NotifyActorEndOverlap(OtherActor);
    DeactivateStealthEffect(); // 은신 구역 없애기
    GetWorld()->GetTimerManager().ClearTimer(StealthTimerHandle);
}
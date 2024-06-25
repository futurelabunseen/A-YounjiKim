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
        // �÷��̾ �� �̻� ���� ���� �� ���� ���� Ȱ��ȭ
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
        // �÷��̾ ���� ���� ���� ���� ��Ȱ��ȭ
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
    // �÷��̾ ���� �������� ������ ��
    Super::NotifyActorEndOverlap(OtherActor);
    DeactivateStealthEffect(); // ���� ���� ���ֱ�
    GetWorld()->GetTimerManager().ClearTimer(StealthTimerHandle);
}
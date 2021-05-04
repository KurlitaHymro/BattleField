// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/BattleFieldGameMode.h"
#include "Common/CommonInterface.h"
#include "Common/BattleFieldDelegation.h"
#include "Interface/CharacterProperty.h"
#include "Character/BattlefieldCharacterPlayer.h"
#include "Controller/PlayerControllerBase.h"

/* 日志类型声明 */
DEFINE_LOG_CATEGORY(LoadLog);
DEFINE_LOG_CATEGORY(RunLog);

UBattleFieldDelegation* ABattleFieldGameMode::Delegation = nullptr;

UCharacterProperty* ABattleFieldGameMode::PropertyTableRow = nullptr;

ABattleFieldGameMode::ABattleFieldGameMode()
{
    UCommonInterface::GameModeInst = this;
    /* 加载代理 */
    Delegation = NewObject<UBattleFieldDelegation>();
    /* 加载数据 */
    PropertyTableRow = NewObject<UCharacterProperty>();
    /* 加载默认配置 */
    DefaultPawnClass = ABattlefieldCharacterPlayer::StaticClass();
    PlayerControllerClass = APlayerControllerBase::StaticClass();
}

ABattleFieldGameMode::~ABattleFieldGameMode()
{
    UCommonInterface::GameModeInst = nullptr;
    /* 卸载 */
    Delegation = nullptr;
    PropertyTableRow = nullptr;
}

UBattleFieldDelegation* ABattleFieldGameMode::GetBattleFieldDelegation()
{
    if (Delegation) {
        return Delegation;
    } else {
        UE_LOG(LoadLog, Error, TEXT("Invalid Delegation"));
        return Delegation = NewObject<UBattleFieldDelegation>();
    }
}

UCharacterProperty* ABattleFieldGameMode::GetCharacterProperty()
{
    if (PropertyTableRow) {
        return PropertyTableRow;
    } else {
        UE_LOG(LoadLog, Error, TEXT("Invalid Delegation"));
        return PropertyTableRow = NewObject<UCharacterProperty>();
    }
}

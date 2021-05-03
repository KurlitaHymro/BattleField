// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/CommonInterface.h"
#include "Common/BattleFieldGameMode.h"
#include "Common/BattleFieldDelegation.h"

// 静态变量类外初始化
ABattleFieldGameMode* UCommonInterface::GameModeInst = nullptr;

ABattleFieldGameMode* UCommonInterface::GetGameModeInst()
{
	return GameModeInst;
}

UBattleFieldDelegation* UCommonInterface::GetDelegation()
{
	return GetGameModeInst()->GetBattleFieldDelegation();
}

UDataTable* UCommonInterface::GetCharacterPropertyTable()
{
	return GetGameModeInst()->GetCharacterProperty()->GetPropertyTableRow();
}

FName UCommonInterface::CombinFNameId(FString name, int id)
{
	FString sId = FString::FromInt(id);
	FString sNameId = name + sId;
	return FName(*sNameId);
}
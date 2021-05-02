// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/CharacterProperty.h"
#include "Engine/DataTable.h"
#include "Common/CommonInterface.h"

bool FPropertyTableRow::operator==(const FPropertyTableRow& other) const
{
    if (CharacterName == other.CharacterName && Rank == other.Rank) {
        return true;
    } else {
        return false;
    }
}

UDataTable* UCharacterProperty::CharacterPropertyTableRow = nullptr;

UCharacterProperty::UCharacterProperty()
{
    LoadCharacterProperty();
    ABattleFieldGameMode::PropertyTableRow = this;
}

UCharacterProperty::~UCharacterProperty()
{
    CharacterPropertyTableRow = nullptr;
    ABattleFieldGameMode::PropertyTableRow = nullptr;
}

UDataTable* UCharacterProperty::GetPropertyTableRow()
{
	if (!CharacterPropertyTableRow) {
		LoadCharacterProperty();
	}
	return CharacterPropertyTableRow;
}

void UCharacterProperty::LoadCharacterProperty(FString dataBase)
{
    FString path = FPaths::Combine(FPaths::ProjectDir(), TEXT("dynPara/" + dataBase + ".csv"));
    FString data;
    if (FFileHelper::LoadFileToString(data, *path)) {
        UE_LOG(LoadLog, Warning, TEXT("Success To Load %s"), *path);
        CharacterPropertyTableRow = NewObject<UDataTable>(GetTransientPackage(), FName(TEXT("UDB_" + dataBase)));
        CharacterPropertyTableRow->RowStruct = FPropertyTableRow::StaticStruct();
        CharacterPropertyTableRow->CreateTableFromCSVString(data);
    }
    else {
        UE_LOG(LoadLog, Error, TEXT("Fail To Load %s"), *path);
    }
}
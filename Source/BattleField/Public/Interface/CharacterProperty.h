// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterProperty.generated.h"

/**
 * 
 */
USTRUCT()
struct BATTLEFIELD_API FPropertyTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	bool operator==(const FPropertyTableRow& other) const;
};

UCLASS()
class BATTLEFIELD_API UCharacterProperty : public UObject
{
	GENERATED_BODY()

	UCharacterProperty();

	~UCharacterProperty();

public:
	static class UDataTable* CharacterPropertyTableRow;

	UFUNCTION(Category = "Interface", BlueprintCallable)
	UDataTable* GetPropertyTableRow();

	UFUNCTION(Category = "DataInterface", BlueprintCallable)
	void LoadCharacterProperty(FString dataBase = "CharacterProperty");
};

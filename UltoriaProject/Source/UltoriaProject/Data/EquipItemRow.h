// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../UI/Menu/EquipTab.h"
#include "EquipItemRow.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FEquipItemRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EEquipSlot Slot;
};
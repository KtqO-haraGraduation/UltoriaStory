// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SequenceMappingRow.generated.h"

USTRUCT(BlueprintType)
struct FSequenceMappingRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString SequenceName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString SequencePath;
};


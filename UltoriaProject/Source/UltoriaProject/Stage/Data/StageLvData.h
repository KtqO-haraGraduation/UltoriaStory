// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../StageParameter.h"
#include "StageLvData.generated.h"

USTRUCT(BlueprintType)
struct FStageLvData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStageType StageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FieldLvName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> FieldLv;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BattleLvName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> BattleLv;
};

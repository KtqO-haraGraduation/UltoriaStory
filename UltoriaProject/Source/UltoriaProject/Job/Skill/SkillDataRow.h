// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillDataRow.generated.h"

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString SkillType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int MpCost;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int EffectValue;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int BreakMult;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int UnLockLv;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int Duration;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Effect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Sound;
};
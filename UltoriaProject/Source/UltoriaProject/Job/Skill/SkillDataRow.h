// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillDataRow.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	NONE,
	ATTACK,
	BUFF,
	HEAL,
};

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	ESkillType SkillType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FName Name;

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
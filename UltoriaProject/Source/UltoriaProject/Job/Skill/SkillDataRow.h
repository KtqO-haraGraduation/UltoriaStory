// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../../Battle/Combat/Data/BattleParameter.h"
#include "SkillDataRow.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	NONE,
	ATTACK,
	BUFF,
	DEBUFF,
	HEAL,
};

UENUM(BlueprintType)
enum class ESkillEffectType : uint8
{
	NONE,
	// 攻撃
	DAMAGE_ATK,
	DAMAGE_MGC,
	
	// バフ
	BUFF_ATK,
	BUFF_DEF,
	BUFF_SPD,

	// デバフ
	DEBUFF_ATK,
	DEBUFF_DEF,
	DEBUFF_SPD,

	// その他
	HEAL_HP,	// HP回復
	CURE_ALL,	// 状態異常回復
	REVIVE,		// 復活
	POISON,		// 毒付与
	TAUNT,		// 攻撃対象の誘導
};

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	ESkillType SkillType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	ESkillEffectType EffectType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETargetTeam TargetTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETargetType TargetType;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Desc;

	bool operator==(const FSkillDataRow& other) const
	{
		return SkillType == other.SkillType;
	}
};

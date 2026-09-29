#pragma once
#include "SkillDataRow.h"

// -------------- 定数 -------------- //
#define SKILL_MAX 6	// ジョブごとのスキル数
// ---------------------------------- //

// ------------- 列挙体 ------------- //

// ---------------------------------- //

// ------------- 構造体 ------------- //
struct FSkillData
{
	ESkillType m_Type;		// スキルタイプ
	FName m_Name;		// 技名
	int   m_MpCost;		// 消費MP
	float m_EffectValue;// 効果量
	float m_BreakMult;	// ブレイク倍率
	int m_UnLockLv;		// 解除レベル
	int m_Duration;		// 効果時間
	FString m_Effect;	// エフェクト
	FString m_Sound;	// サウンド
};
// ---------------------------------- //
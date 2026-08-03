#pragma once
#include "Skill/SkillParameter.h"

struct FSkillData;

// -------------- 定数 -------------- //
#define JOB_LEVEL_MAX 10	// ジョブの最大レベル
// ---------------------------------- //

// ------------- 列挙体 ------------- //
enum JobID
{
	FENCER,		// 剣士
	MAGICIAN,	// 魔法使い
	JOB_MAX,
	JOB_NONE = -1,
};
// ---------------------------------- //

// ------------- 構造体 ------------- //
// データテーブル以外のデータ
struct FJobBase
{
	FSkillData* m_SkillData[SKILL_MAX];
	int m_Lv;	// レベル
	int m_Exp;	// 経験値
};

// ジョブデータの構造体
struct FJobData
{
	FString m_Name;		// 名前
	float	m_HP;		// 体力
	float	m_MP;		// マナ
	float	m_ATK;		// 攻撃力
	float	m_MGC;		// 魔力
	float	m_DEF;		// 防御力
	float	m_SPD;		// 素早さ
	float	m_Break;
	float	m_BreakGauge;
};
// ---------------------------------- //
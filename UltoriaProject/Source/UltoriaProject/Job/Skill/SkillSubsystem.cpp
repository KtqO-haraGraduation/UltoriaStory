// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSubsystem.h"
#include "SkillDataAsset.h"
#include "SkillDataRow.h"
#include <ioStream>
#include <string>

#define STATUS_DIAMETER 100.0f // 倍率

using namespace std;

USkillSubsystem::USkillSubsystem()
{
	for (int i = FENCER; i < JOB_MAX; i++)
	{
		for (int j = 0; j < SKILL_MAX; j++)
		{
			m_SkillData[i][j].m_Type = ESkillType::NONE;
			m_SkillData[i][j].m_Name = "";
			m_SkillData[i][j].m_MpCost = 0;
			m_SkillData[i][j].m_EffectValue = 0;
			m_SkillData[i][j].m_BreakMult = 0;
			m_SkillData[i][j].m_UnLockLv = 0;
			m_SkillData[i][j].m_Duration = 0;
			m_SkillData[i][j].m_Effect = "";
			m_SkillData[i][j].m_Sound = "";
		}
	}
}

void USkillSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//LoadSkillDataAsset();

	//if (!m_SkillDA)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("SkillDataAsset/false"));
	//	return;
	//}

	//// 初期化
	//m_Rows.Empty();
	//// 全スキルデータを取得
	//m_SkillDA->m_SkillMappingTable->GetAllRows<FSkillDataRow>(TEXT(""), m_Rows);

	//for (int i = FENCER; i < JOB_MAX; i++)
	//{
	//	for (int j = 0; j < SKILL_MAX; j++)
	//	{
	//		// スキルデータID = ジョブID * ジョブごとに持てるスキル数 + スキルID
	//		// FENCER(0)・・・0～5, MAGICIAN(1)・・・6～11
	//		int k = i * SKILL_MAX + j;

	//		// スキルデータの要素数を超えたか
	//		// falseが呼ばれると超えている
	//		if (!m_Rows.IsValidIndex(k))
	//		{
	//			continue;	// 超えていたら生成処理をスルー
	//		}

	//		m_SkillData[i][j] = CreateSkillData(k);	// スキルデータを生成
	//	}
	//}
}

void USkillSubsystem::LoadSkillDataAsset()
{
	// TEXTの内容・・・DA_SkillDataのリファレンス
	m_SkillDA = LoadObject<USkillDataAsset>(nullptr, TEXT("/Script/UltoriaProject.SkillDataAsset'/Game/MembarFiles/Sakumoto/DataAsset/DA_SkillData.DA_SkillData'"));
}

/// <summary>
/// スキルデータを生成
/// </summary>
/// <param name="id">スキルデータID</param>
/// <returns>生成したデータを返す</returns>
FSkillData USkillSubsystem::CreateSkillData(int id)
{
	FSkillData skillData;

	skillData.m_Type		= m_Rows[id]->SkillType;
	//skillData.m_Name		= m_Rows[id]->Name;
	skillData.m_MpCost		= m_Rows[id]->MpCost;
	skillData.m_EffectValue = m_Rows[id]->EffectValue	/ STATUS_DIAMETER;
	skillData.m_BreakMult	= m_Rows[id]->BreakMult		/ STATUS_DIAMETER;
	skillData.m_UnLockLv	= m_Rows[id]->UnLockLv;
	skillData.m_Duration	= m_Rows[id]->Duration;
	skillData.m_Effect		= m_Rows[id]->Effect;
	skillData.m_Sound		= m_Rows[id]->Sound;

	return skillData;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../Enemy/EnemyBase.h"
#include "EnemyMasterData.h"
#include "AllyLevelData.h"
#include "AllyMasterData.h"
#include "../../Battle/Combat/Data/EnemyPartyData.h"
#include "../../Job/Skill/SkillDataRow.h"
#include "../../Job/JobDataRow.h"
#include "../../Job/JobLvRow.h"
#include "CharacterDataManager.generated.h"

class UCharacterDataAsset;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCharacterDataManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	// ロード関数
	void LoadCharacterDataAsset();

	void LoadAllyMasterData();
	void LoadAllyLevelData();
	
	void LoadEnemyData();
	void LoadEnemyPartyData();

	void LoadSkillData();

	void LoadJobData();
	void LoadJobLvData();

	// Get関数
	// Idで指定した仲間のステータスデータを取得 
	FAllyLevelData GetAllyLevelData(FName allyId, int Lv) const;
	FAllyMasterData GetAllyMasterData(FName allyId) const;

	// Idで指定した敵のデータを取得
	FEnemyMasterData GetEnemyData(FName enemyId) const;
	// 敵のデータからクラスを検索して取得
	TSubclassOf<AEnemyBase> GetEnemyClass(FName enemyId);

	FEnemyPartyData GetRandomEnemyParty(FName enemyId);
	// 敵のIDを元にパーティー候補を検索して返す
	TArray<FEnemyPartyData> FindEnemyParty(FName enemyId);

	// 名前で指定したスキルのデータを取得
	FSkillDataRow GetSkillData(FName name);

	// 名前で指定したジョブのデータ取得
	FJobDataRow GetJobData(FName jobName);
	// ジョブのLvのExp値を検索して返す
	FJobLvRow GetJobLvData(int nexJobLv);

private:
	UDataTable* GetEnemyPartyTable();

private:
	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> m_CharacterDataAsset; // キャラクターのデータアセット

	TMap<FName, FAllyLevelData> m_AllyLevelDatas;		// 仲間のデータ
	TMap<FName, FAllyMasterData> m_AllyMasterDatas;		// 仲間の設定データ

	TMap<FName, FEnemyMasterData> m_EnemyDatas;			// 敵のデータ
	TArray<FEnemyPartyData> m_EnemyPartyDatas;			// ステージに対応した敵のパーティーデータ
	FEnemyPartyData m_BossPartyData;					// ボスのパーティーデータ

	TMap<FName, FSkillDataRow> m_SkillDatas;			// スキルのデータ

	TMap<FName, FJobDataRow> m_JobDatas;				// ジョブのデータ
	TMap<int, FJobLvRow> m_JobLvDatas;					// ジョブLvのデータ
};

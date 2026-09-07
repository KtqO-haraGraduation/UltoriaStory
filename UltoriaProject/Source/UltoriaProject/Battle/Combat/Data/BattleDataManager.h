// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyPartyData.h"
#include "BattleCommandData.h"
#include "BattleStatusData.h"
#include "Subsystems/WorldSubsystem.h"
#include "BattleDataManager.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleDataManager : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:
	// ロード
	// 現在のステージの敵データをロード
	void LoadEnemyPartyData();

	void LoadCommandData();
	void LoadStatusData();

	// Get関数
	FEnemyPartyData GetRandomEnemyParty(FName enemyId);

	const TArray<FBattleCommandData>& GetCommandDatas() const { return m_CommandDatas; }
	const TArray<FBattleStatusData>& GetStatusDatas() const { return m_StatusDatas; }

	// 指定したステータスのデータを返す
	const FBattleStatusData FindStatusData(StatusType type);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 現在のステージに対応した敵のパーティーデータのパスを取得
	FSoftObjectPath GetEnemyPartyDataPath(EStageType type);

	// 敵のIDを元にパーティー候補を検索して返す
	TArray<FEnemyPartyData> FindEnemyParty(FName enemyId);

private:
	// ステージごとの敵DataTable
	UPROPERTY()
	TMap<EStageType, FSoftObjectPath> m_EnemyPartyTablePaths;

	TArray<FEnemyPartyData> m_EnemyPartyDatas;	// ステージに対応した敵のパーティーデータ
	FEnemyPartyData m_BossPartyData;			// ボスのパーティーデータ

	TArray<FBattleCommandData> m_CommandDatas;	// コマンドデータ

	TArray<FBattleStatusData> m_StatusDatas;	// ステータスデータ
};

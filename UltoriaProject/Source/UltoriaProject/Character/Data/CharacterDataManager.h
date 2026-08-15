// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../Enemy/EnemyBase.h"
#include "EnemyMasterData.h"
#include "AllyLevelData.h"
#include "AllyMasterData.h"
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

	// Get関数
	// Idで指定した仲間のステータスデータを取得 
	FAllyLevelData GetAllyLevelData(FName allyId, int Lv) const;
	FAllyMasterData GetAllyMasterData(FName allyId) const;

	// Idで指定した敵のデータを取得
	FEnemyMasterData GetEnemyData(FName enemyId) const;
	// 敵のデータからクラスを検索して取得
	TSubclassOf<AEnemyBase> GetEnemyClass(FName enemyId);

private:
	bool IsCharacterDataAssetLoaded();

private:
	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> m_CharacterDataAsset; // キャラクターのデータアセット

	TMap<FName, FAllyLevelData> m_AllyLevelDatas;		// 仲間のデータ
	TMap<FName, FAllyMasterData> m_AllyMasterDatas;		// 仲間の設定データ
	TMap<FName, FEnemyMasterData> m_EnemyDatas;			// 敵のデータ
};

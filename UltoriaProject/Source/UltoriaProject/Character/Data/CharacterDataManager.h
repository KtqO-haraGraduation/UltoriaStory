// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../Enemy/EnemyBase.h"
#include "EnemyMasterData.h"
#include "CharacterDataManager.generated.h"

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
	void LoadEnemyData();

	// Get関数
	// Idで指定した敵のデータを取得
	FEnemyMasterData GetEnemyData(FName enemyId) const;
	// 敵のデータからクラスを検索して取得
	TSubclassOf<AEnemyBase> GetEnemyClass(FName enemyId);

private:
	TMap<FName, FEnemyMasterData> m_EnemyDatas; // 敵のデータ
};

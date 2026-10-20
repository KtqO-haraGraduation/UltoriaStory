// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleCommandData.h"
#include "BattleStatusData.h"
#include "Subsystems/WorldSubsystem.h"
#include "BattleDataManager.generated.h"

class UBattleDataAsset;
class UBattleUIDataAsset;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleDataManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// ロード
	void LoadBattleDataAsset();
	void LoadBattleUIDataAsset();

	void LoadCommandData();
	void LoadStatusData();

	const UBattleUIDataAsset* GetBattleUIDataAsset() const { return m_BattleUIDataAsset; }
	const TArray<FBattleCommandData>& GetCommandDatas() const { return m_CommandDatas; }

	// 指定したステータスのデータを返す
	const FBattleStatusData FindStatusData(StatusType type);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	TObjectPtr<UBattleDataAsset> m_BattleDataAsset;		// バトルのデータアセット
	TObjectPtr<UBattleUIDataAsset> m_BattleUIDataAsset; // バトルUIのデータアセット

	TArray<FBattleCommandData> m_CommandDatas;	// コマンドデータ
	TArray<FBattleStatusData> m_StatusDatas;	// ステータスデータ
};

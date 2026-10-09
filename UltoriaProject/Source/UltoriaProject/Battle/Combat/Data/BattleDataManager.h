// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void LoadCommandData();
	void LoadStatusData();

	const TArray<FBattleCommandData>& GetCommandDatas() const { return m_CommandDatas; }
	const TArray<FBattleStatusData>& GetStatusDatas() const { return m_StatusDatas; }

	// 指定したステータスのデータを返す
	const FBattleStatusData FindStatusData(StatusType type);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	TArray<FBattleCommandData> m_CommandDatas;	// コマンドデータ
	TArray<FBattleStatusData> m_StatusDatas;	// ステータスデータ
};

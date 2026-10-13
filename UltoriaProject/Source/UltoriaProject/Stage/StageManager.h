// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/StageLvData.h"
#include "StageParameter.h"
#include "StageManager.generated.h"

//class FStageLvData;
class UStageDataAsset;
class ULevelStreamingDynamic;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UStageManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadStageDataAsset();
	void LoadStageLvData();

	// ステージの切り替え
	void ChangeStage(EStageType type, FName id);

	// IDを元に、StageLvDataを返す
	FStageLvData GetNowStageLvData();      // 現在開いているLvのデータを取得
	FStageLvData GetStageLvData(FName id); // 指定したIDのLvのデータを取得

	EStageType GetStageType() const { return m_StageType; }
	FName GetStageName() const { return m_StageName; }

	void SetStageType(EStageType type) { if(m_StageType != type) m_StageType = type; }
	void SetStageName(FName name) { if(name != "") m_StageName = name; }

private:
	bool IsStageDataAssetLoaded();
	ULevelStreamingDynamic* LoadLevel(TSoftObjectPtr<UWorld> level);

private:
	EStageType m_StageType = EStageType::FOREST;  // 現在のステージタイプ
	FName m_StageName = "Test";					  // 現在のステージの名前（ID）

	UPROPERTY()
	ULevelStreaming* m_CurrentFieldLv;
	UPROPERTY()
	ULevelStreaming* m_CurrentBattleLv;
	

	UPROPERTY()
	TObjectPtr<UStageDataAsset> m_StageDataAsset; // ステージのデータアセット
	TMap<FName, FStageLvData> m_StageLvDatas;	  // 各ステージのFieldとBattleLvのデータ
};

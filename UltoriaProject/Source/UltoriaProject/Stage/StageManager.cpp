// Fill out your copyright notice in the Description page of Project Settings.

#include "StageManager.h"
#include "Data/StageDataAsset.h"
#include "Data/StageLvData.h"
#include "Engine/LevelStreamingDynamic.h"

void UStageManager::Initialize(FSubsystemCollectionBase& Collection)
{
	// ステージのデータアセットをロード
	LoadStageDataAsset();

	// 必要なステージデータをロード
	LoadStageLvData();

	//ChangeStage(EStageType::TUTORIAL, "Tutorial");
}

void UStageManager::LoadStageDataAsset()
{
	m_StageDataAsset = LoadObject<UStageDataAsset>(
		nullptr,
		TEXT("/Game/Data/Lv/DA_Stage.DA_Stage")
	);
}

void UStageManager::LoadStageLvData()
{
	if (!IsStageDataAssetLoaded()) return;

	UDataTable* table = m_StageDataAsset->StageLvTable;
	if (!table) return;

	m_StageLvDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FStageLvData* data =
			reinterpret_cast<FStageLvData*>(row.Value);

		if (data)
		{
			m_StageLvDatas.Add(data->Name, *data);
		}
	}
}

void UStageManager::ChangeStage(EStageType type, FName id)
{
	const FStageLvData* data = m_StageLvDatas.Find(id);
	if (!data) return;

	m_StageType = type;
	m_StageName = id;

	// 現在のLvをUnload
	if (m_CurrentFieldLv)
	{
		m_CurrentFieldLv->SetShouldBeVisible(false);
		m_CurrentFieldLv->SetShouldBeLoaded(false);
		m_CurrentFieldLv = nullptr;
	}

	if (m_CurrentBattleLv)
	{
		m_CurrentBattleLv->SetShouldBeVisible(false);
		m_CurrentBattleLv->SetShouldBeLoaded(false);
		m_CurrentBattleLv = nullptr;
	}

	// 新しいLvを作成
	m_CurrentFieldLv = LoadLevel(data->FieldLv);
	m_CurrentBattleLv = LoadLevel(data->BattleLv);

	// バトルLvは非表示
	m_CurrentBattleLv->SetShouldBeVisible(false);
}

FStageLvData UStageManager::GetNowStageLvData()
{
	const FStageLvData* data = m_StageLvDatas.Find(m_StageName);
	if (data)
	{
		return *data;
	}
	return FStageLvData();
}

FStageLvData UStageManager::GetStageLvData(FName id)
{
	const FStageLvData* data = m_StageLvDatas.Find(id);
	if (data)
	{
		return *data;
	}
	return FStageLvData();
}

bool UStageManager::IsStageDataAssetLoaded()
{
	if (!m_StageDataAsset) return false;
	return true;
}

ULevelStreamingDynamic* UStageManager::LoadLevel(TSoftObjectPtr<UWorld> level)
{
	if (level.IsNull()) return nullptr;

	bool bSuccess = false;

	ULevelStreamingDynamic* streamingLevel =
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
			GetWorld(),
			level,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			bSuccess,
			TEXT(""),
			nullptr,
			false
		);

	streamingLevel->SetShouldBeLoaded(true);
	streamingLevel->SetShouldBeVisible(true);

	return streamingLevel;
}


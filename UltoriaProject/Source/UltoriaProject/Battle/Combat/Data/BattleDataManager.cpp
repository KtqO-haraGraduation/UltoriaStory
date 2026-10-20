// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleDataManager.h"
#include "BattleDataAsset.h"
#include "../../../UI/Battle/Data/BattleUIDataAsset.h"

void UBattleDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// バトルのデータアセットをロード
	LoadBattleDataAsset();
	LoadBattleUIDataAsset();

	LoadCommandData();
	LoadStatusData();
}

void UBattleDataManager::LoadBattleDataAsset()
{
	m_BattleDataAsset = LoadObject<UBattleDataAsset>(
		nullptr,
		TEXT("/Game/Data/Battle/DA_Battle.DA_Battle")
	);
}

void UBattleDataManager::LoadBattleUIDataAsset()
{
	m_BattleUIDataAsset = LoadObject<UBattleUIDataAsset>(
		nullptr,
		TEXT("/Game/Data/UI/Battle/DA_BattleUI.DA_BattleUI")
	);
}

void UBattleDataManager::LoadCommandData()
{
	UDataTable* table = m_BattleDataAsset->CommandTable;
	if (!table) return;

	m_CommandDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FBattleCommandData* data = reinterpret_cast<FBattleCommandData*>(row.Value);

		if (data)
		{
			FBattleCommandData copy = *data;
			copy.Name = rowName;

			m_CommandDatas.Add(copy);
		}
	}
}

void UBattleDataManager::LoadStatusData()
{
	UDataTable* table = m_BattleDataAsset->StatusTable;
	if (!table) return;

	m_StatusDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FBattleStatusData* data = reinterpret_cast<FBattleStatusData*>(row.Value);

		if (data)
		{
			FBattleStatusData copy = *data;
			copy.id = rowName;

			m_StatusDatas.Add(copy);
		}
	}
}

const FBattleStatusData UBattleDataManager::FindStatusData(StatusType type)
{
	FBattleStatusData result;

	for (const FBattleStatusData& data : m_StatusDatas)
	{
		if (data.type == type)
		{
			result = data;
		}
	}

	return result;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataManager.h"
#include "CharacterDataAsset.h"

void UCharacterDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// キャラクターのデータアセットをロード
	LoadCharacterDataAsset();

	// 必要なキャラデータをロード
	LoadAllyMasterData();
	LoadAllyLevelData();
	LoadEnemyData();
}

void UCharacterDataManager::LoadCharacterDataAsset()
{
	m_CharacterDataAsset = LoadObject<UCharacterDataAsset>(
		nullptr,
		TEXT("/Game/Data/Character/DA_Character.DA_Character")
	);
}

void UCharacterDataManager::LoadAllyMasterData()
{
	if (!IsCharacterDataAssetLoaded()) return;

	UDataTable* table = m_CharacterDataAsset->AllyTable;
	if (!table) return;

	m_AllyMasterDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FAllyMasterData* data = reinterpret_cast<FAllyMasterData*>(row.Value);

		if (data)
		{
			m_AllyMasterDatas.Add(rowName, *data);
		}
	}
}

void UCharacterDataManager::LoadAllyLevelData()
{
	if (!IsCharacterDataAssetLoaded()) return;

	UDataTable* table = m_CharacterDataAsset->AllyLevelTable;
	if (!table) return;

	m_AllyLevelDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FAllyLevelData* data = reinterpret_cast<FAllyLevelData*>(row.Value);

		if (data)
		{
			m_AllyLevelDatas.Add(rowName, *data);
		}
	}
}

void UCharacterDataManager::LoadEnemyData()
{
	if (!IsCharacterDataAssetLoaded()) return;

	UDataTable* table = m_CharacterDataAsset->EnemyTable;
	if (!table) return;

	m_EnemyDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FEnemyMasterData* data = reinterpret_cast<FEnemyMasterData*>(row.Value);

		if (data)
		{
			m_EnemyDatas.Add(rowName, *data);
		}
	}
}

FAllyLevelData UCharacterDataManager::GetAllyLevelData(FName allyId, int Lv) const
{
	FName rowName(
		*FString::Printf(TEXT("%s_%02d"), *allyId.ToString(), Lv)
	);

	const FAllyLevelData* data = m_AllyLevelDatas.Find(rowName);

	if (data)
	{
		return *data;
	}
	return FAllyLevelData();
}

FAllyMasterData UCharacterDataManager::GetAllyMasterData(FName allyId) const
{
	const FAllyMasterData* data = m_AllyMasterDatas.Find(allyId);

	if (data)
	{
		return *data;
	}
	return FAllyMasterData();
}

FEnemyMasterData UCharacterDataManager::GetEnemyData(FName enemyId) const
{
	const FEnemyMasterData* data = m_EnemyDatas.Find(enemyId);

	if (data)
	{
		return *data;
	}
	return FEnemyMasterData();
}

TSubclassOf<AEnemyBase> UCharacterDataManager::GetEnemyClass(FName enemyId)
{
	const FEnemyMasterData* data = m_EnemyDatas.Find(enemyId);

	if (data)
	{
		return data->EnemyClass;
	}
	return nullptr;
}

bool UCharacterDataManager::IsCharacterDataAssetLoaded()
{
	if (!m_CharacterDataAsset)
	{
		return false;
	}
	return true;
}

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

/// <summary>
/// キャラクターのデータアセットをロード
/// </summary>
void UCharacterDataManager::LoadCharacterDataAsset()
{
	m_CharacterDataAsset = LoadObject<UCharacterDataAsset>(
		nullptr,
		TEXT("/Game/Data/Character/DA_Character.DA_Character")
	);
}

/// <summary>
/// プレイヤーと仲間のマスターデータをロード
/// </summary>
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

/// <summary>
/// 仲間のLVごとのデータをロード
/// </summary>
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

/// <summary>
/// 敵のデータをロード
/// </summary>
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

/// <summary>
/// 指定した仲間のLVデータを取得
/// </summary>
/// <param name="allyId">キャラクターID</param>
/// <param name="Lv">ほしいデータのLV</param>
/// <returns>指定したLVでデータ</returns>
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

/// <summary>
/// 指定した仲間のマスターデータを取得
/// </summary>
/// <param name="allyId">敵のID</param>
/// <returns>指定した仲間のマスターデータ</returns>
FAllyMasterData UCharacterDataManager::GetAllyMasterData(FName allyId) const
{
	const FAllyMasterData* data = m_AllyMasterDatas.Find(allyId);

	if (data)
	{
		return *data;
	}
	return FAllyMasterData();
}

/// <summary>
/// 指定した敵のデータを取得
/// </summary>
/// <param name="enemyId">敵のID</param>
/// <returns>指定した敵のデータ</returns>
FEnemyMasterData UCharacterDataManager::GetEnemyData(FName enemyId) const
{
	const FEnemyMasterData* data = m_EnemyDatas.Find(enemyId);

	if (data)
	{
		return *data;
	}
	return FEnemyMasterData();
}

/// <summary>
/// 指定した敵クラスを取得
/// </summary>
/// <param name="enemyId">敵のID</param>
/// <returns>指定した敵のクラス</returns>
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataManager.h"

void UCharacterDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 敵のデータをロード
	LoadEnemyData();
}

void UCharacterDataManager::LoadEnemyData()
{
	FSoftObjectPath path = FSoftObjectPath("/Game/MembarFiles/Matsubara/Data_MC/EnemyData/DT_EnemyMaster.DT_EnemyMaster");
	UDataTable* table = Cast<UDataTable>(path.TryLoad());
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

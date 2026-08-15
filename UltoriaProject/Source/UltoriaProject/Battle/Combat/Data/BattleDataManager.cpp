// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleDataManager.h"



void UBattleDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	m_EnemyPartyTablePaths.Add(EStageType::FOREST,		 FSoftObjectPath("/Game/Data/Character/Enemy/Party/DT_Forest_EnemyParty.DT_Forest_EnemyParty"));
	m_EnemyPartyTablePaths.Add(EStageType::CAVE,		 FSoftObjectPath(""));
	m_EnemyPartyTablePaths.Add(EStageType::POISON_SWAMP, FSoftObjectPath(""));
	m_EnemyPartyTablePaths.Add(EStageType::CASTLE,		 FSoftObjectPath(""));

	LoadStatusData();
}

void UBattleDataManager::LoadEnemyPartyData()
{
	UStageManager* stageManager = GetWorld()->GetSubsystem<UStageManager>();

	FSoftObjectPath path = GetEnemyPartyDataPath(stageManager->GetStageType());
	UDataTable* table = Cast<UDataTable>(path.TryLoad());
	if (!table) return;

	m_EnemyPartyDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FEnemyPartyData* data = reinterpret_cast<FEnemyPartyData*>(row.Value);

		if (data)
		{
			FEnemyPartyData copy = *data;

			copy.Name = rowName;

			m_EnemyPartyDatas.Add(copy);
		}
	}
}

void UBattleDataManager::LoadCommandData()
{
	FSoftObjectPath path = FSoftObjectPath("/Game/Data/Battle/DT_BattleCommand.DT_BattleCommand");
	UDataTable* table = Cast<UDataTable>(path.TryLoad());
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
	FSoftObjectPath path = FSoftObjectPath("/Game/Data/Character/Status/DT_CharacterStatus.DT_CharacterStatus");
	UDataTable* table = Cast<UDataTable>(path.TryLoad());
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

const FBattleStatusData UBattleDataManager::FindStatusData(FName statusID)
{
	FBattleStatusData result;

	for (const FBattleStatusData& data : m_StatusDatas)
	{
		if (data.id == statusID)
		{
			result = data;
		}
	}

	return result;
}

FSoftObjectPath UBattleDataManager::GetEnemyPartyDataPath(EStageType type)
{
	FSoftObjectPath* path = m_EnemyPartyTablePaths.Find(type);

	if (path)
	{
		return *path;
	}

	return FSoftObjectPath();
}

FEnemyPartyData UBattleDataManager::GetRandomEnemyParty(FName enemyId)
{
	TArray<FEnemyPartyData> candidates =
		FindEnemyParty(enemyId);

	if (candidates.Num() == 0)
	{
		return FEnemyPartyData();
	}

	int index = FMath::RandRange(0, candidates.Num() - 1);

	return candidates[index];
}

TArray<FEnemyPartyData> UBattleDataManager::FindEnemyParty(FName enemyId)
{
	TArray<FEnemyPartyData> result;

	for (const FEnemyPartyData& data : m_EnemyPartyDatas)
	{
		if (data.Enemy1 == enemyId ||
			data.Enemy2 == enemyId ||
			data.Enemy3 == enemyId ||
			data.Enemy4 == enemyId)
		{
			result.Add(data);
		}
	}

	return result;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleDataManager.h"

void UBattleDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadStatusData();
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

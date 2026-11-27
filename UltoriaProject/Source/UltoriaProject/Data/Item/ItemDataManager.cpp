// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDataManager.h"
#include "../../System/GameInstance/UltoriaGameInstance.h"

void UItemDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadItemData();
}

/// <summary>
/// アイテムのデータをロード
/// </summary>
void UItemDataManager::LoadItemData()
{
	UUltoriaGameInstance* gameInstance = GetWorld()->GetGameInstance<UUltoriaGameInstance>();
	if (!gameInstance) return;
	UDataTable* table = gameInstance->ItemDataTable;
	if (!table) return;

	m_ItemDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FItemData* data = reinterpret_cast<FItemData*>(row.Value);
		FName id = data->ID;

		if (data)
		{
			m_ItemDatas.Add(id, *data);
		}
	}
}

/// <summary>
/// 指定したアイテムのデータを取得
/// </summary>
/// <param name="itemId">アイテムのID</param>
/// <returns>指定したアイテムのデータ</returns>
const FItemData* UItemDataManager::GetItemData(FName itemId) const
{
	return m_ItemDatas.Find(itemId);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManager.h"

void UEquipmentManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_EquipMasterData = LoadObject<UDataTable>(nullptr,
		TEXT("/Game/MembarFiles/Yoshihiro/Data_Yoshi/Equip/DA_EquipMasterData"));

	// テスト用所持データ
	m_OwnedEquipIds.Add(FName("row_1"));
	m_OwnedEquipIds.Add(FName("row_2"));
	m_OwnedEquipIds.Add(FName("row_3"));
	m_OwnedEquipIds.Add(FName("row_4"));
}

FEquipItemRow* UEquipmentManager::GetEquipData(FName id)
{
	if (!m_EquipMasterData)return nullptr;
	return m_EquipMasterData->FindRow<FEquipItemRow>(id, TEXT(""));
}

TArray<FName> UEquipmentManager::GetOwnedItemsBySlot(EEquipSlot Slot)
{
	TArray<FName> result;
	for (FName id : m_OwnedEquipIds)
	{
		FEquipItemRow* data = GetEquipData(id);
		if (data && data->Slot == Slot)
		{
			result.Add(id);
		}
	}
	return result;
}

void UEquipmentManager::AddOwnedEquip(FName id)
{
	if (!m_OwnedEquipIds.Contains(id))
	{
		m_OwnedEquipIds.Add(id);
	}
}

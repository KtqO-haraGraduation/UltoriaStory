// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EquipItemRow.h"
#include "EquipmentManager.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UEquipmentManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	// IDからマスターデータを検索して取得
	FEquipItemRow* GetEquipData(FName id);

	// 装備箇所でフィルタして所持装備IDリストを返す
	TArray<FName>GetOwnedItemsBySlot(EEquipSlot Slot);

	// 所持装備を追加
	void AddOwnedEquip(FName id);

	// 所持装備リストを返す
	const TArray<FName>& GetOwnedEquipds() const { return m_OwnedEquipIds; }

private:
	// 装備のマスターデータ
	UPROPERTY()
	UDataTable* m_EquipMasterData;

	// 所持装備リスト
	TArray<FName> m_OwnedEquipIds;
};

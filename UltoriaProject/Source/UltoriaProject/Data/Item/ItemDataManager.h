// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../../Akiduki/ItemData/ItemData.h"
#include "ItemDataManager.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UItemDataManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	// ÉçÅ[Éh
	void LoadItemData();

	FItemData GetItemData(FName itemId) const;

private:
	TMap<FName, FItemData> m_ItemDatas;
};

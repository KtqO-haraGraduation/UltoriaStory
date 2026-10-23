// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventorySubsystem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    // アイテムID
    UPROPERTY(BlueprintReadOnly)
    FName ItemID;

    // 所持数
    UPROPERTY(BlueprintReadOnly)
    int32 Count = 0;

    bool operator==(const FInventoryItem& other) const
    {
        return ItemID == other.ItemID;
    }
};

UCLASS()
class ULTORIAPROJECT_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // アイテム取得
    void AddItem(FName ItemID, int32 Count = 1);

    // アイテム消費
    bool RemoveItem(FName ItemID, int32 Count = 1);

    // 所持数取得
    int32 GetItemCount(FName ItemID) const;

    // 持っているか
    bool HasItem(FName ItemID, int32 Count = 1) const;

    // 所持品一覧取得
    const TArray<FInventoryItem>& GetInventory() const { return Inventory; }

    // インベントリをクリア
    void ClearInventory();

private:

    UPROPERTY()
    TArray<FInventoryItem> Inventory;
};

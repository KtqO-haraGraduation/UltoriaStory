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

    // 所持金取得
    int32 GetMoney() const { return Money; }

    // 所持金を追加
    void AddMoney(int32 Amount);

    // 所持金を消費
    bool RemoveMoney(int32 Amount);

    // 所持金を設定
    void SetMoney(int32 Amount);
private:

	// インベントリ
    UPROPERTY()
    TArray<FInventoryItem> Inventory;

    // 所持金
    UPROPERTY()
    int32 Money;
};

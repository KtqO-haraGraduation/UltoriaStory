// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem.h"

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Inventory.Empty();
}

void UInventorySubsystem::AddItem(FName ItemID, int32 Count)
{
    for (FInventoryItem& Item : Inventory)
    {
        // IDが一致したらアイテム数を追加
        if (Item.ItemID == ItemID)
        {
            Item.Count += Count;
            return;
        }
    }

	// 新しいアイテムを追加
    FInventoryItem NewItem;
    NewItem.ItemID = ItemID;
    NewItem.Count = Count;

	Inventory.Add(NewItem);// インベントリに追加

    // ログ表示
    UE_LOG(LogTemp, Warning,
        TEXT("AddItem : %s x%d"),
        *ItemID.ToString(),
        GetItemCount(ItemID));
}

bool UInventorySubsystem::RemoveItem(FName ItemID, int32 Count)
{
    for (int32 i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i].ItemID == ItemID)
        {
			// アイテム数が足りない場合は削除できない
            if (Inventory[i].Count < Count)
            {
                return false;
            }

            // アイテム数を減らす
			Inventory[i].Count -= Count;

			// アイテム数が0以下になったらインベントリから削除
            if (Inventory[i].Count <= 0)
            {
                Inventory.RemoveAt(i);
            }

            return true;
        }
    }

    return false;
}

int32 UInventorySubsystem::GetItemCount(FName ItemID) const
{
	// アイテムIDが一致するアイテムの所持数を返す
    for (const FInventoryItem& Item : Inventory)
    {
        if (Item.ItemID == ItemID)
        {
            return Item.Count;
        }
    }

    return 0;
}

bool UInventorySubsystem::HasItem(FName ItemID, int32 Count) const
{
	// アイテムIDが一致するアイテムの所持数が指定された数以上かどうかを返す
    return GetItemCount(ItemID) >= Count;
}

void UInventorySubsystem::ClearInventory()
{
    Inventory.Empty();
}

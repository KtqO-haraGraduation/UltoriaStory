// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "../../Inventory/UI/InventoryItemEntryWidget.h"
#include "../../Inventory/UI/ItemUseConfirmWidget.h"
#include "../../Inventory/InventorySubsystem.h"
#include "../../Akiduki/ItemData/ItemData.h"
#include "../../Akiduki/ItemEffect/ItemUseSubsystem.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include <UltoriaProject/System/GameInstance/UltoriaGameInstance.h>
#include <UltoriaProject/UI/Menu/MenuStateManager.h>

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Inventoryのアイテム一覧を作成する
	SetupItems();

	// 所持金更新
	UpdateMoney();
}

void UInventoryWidget::NavigateUp()
{
	// 上方向へ1つ選択を移動する
	MoveSelection(-1);
}

void UInventoryWidget::NavigateDown()
{
	// 下方向へ1つ選択を移動する
	MoveSelection(1);
}

void UInventoryWidget::Confirm()
{
	// 確認画面がすでに開いているなら、2回目の決定
	if (bIsConfirmingItemUse)
	{
		ExecutePendingItemUse();
		return;
	}

	// 現在選択されているIndexが有効か確認する
	if (!ItemEntries.IsValidIndex(CurrentIndex))
	{
		return;
	}

	// 現在選択されているEntryを取得する
	UInventoryItemEntryWidget* SelectedEntry = ItemEntries[CurrentIndex];

	// 選択中のアイテムデータを取得する
	const FItemData& ItemData = SelectedEntry->GetItemData();

	// 選択したアイテムをログに表示する
	UE_LOG(LogTemp,Log,TEXT("Inventory Confirm : %s"),*ItemData.ID.ToString());
	
	// 消耗品以外は使用しない
	if (ItemData.Type != EItemType::Consumable)
	{
		UE_LOG(LogTemp, Log, TEXT("Item cannot be used"));
		return;
	}

	// フィールドで使用できないアイテムは処理を終了
	if (!ItemData.bUsableInField)
	{
		UE_LOG(LogTemp, Log, TEXT("Cannot be used in field"));
		return;
	}

	// 効果が設定されていないアイテムは処理を終了
	if (ItemData.Effects.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No item effects configured"));
		return;
	}

	// 使用対象の設定をログに出力
	UE_LOG(LogTemp,Log,TEXT("Item use target: %d"),static_cast<int32>(ItemData.Target));
	// インベントリを取得
	UGameInstance* GI = GetGameInstance();
	// InventorySubsystemを取得
	UInventorySubsystem* Inventory = GI->GetSubsystem<UInventorySubsystem>();
	// アイテムを所持しているか確認
	if (!Inventory->HasItem(ItemData.ID, 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item not found in inventory"));
		SetupItems();// 所持アイテム一覧を更新
		return;
	}

	// 選択したアイテムを保存して確認画面を開く
	OpenItemUseConfirm(ItemData);
}

void UInventoryWidget::Cancel()
{
	// アイテム使用の確認中なら、確認画面だけ閉じる
	if (bIsConfirmingItemUse)
	{
		if (IsValid(ItemUseConfirmWidget))
		{
			ItemUseConfirmWidget->RemoveFromParent();
			ItemUseConfirmWidget = nullptr;
		}

		bIsConfirmingItemUse = false;
		return;
	}

	// メニューを管理しているMenuStateManagerを取得する
	UMenuStateManager* MenuManager =
		GetGameInstance()->GetSubsystem<UMenuStateManager>();

	// ひとつ前のメニューに戻る
	if (IsValid(MenuManager))
	{
		MenuManager->BackMenu();
	}
}

void UInventoryWidget::MoveSelection(int32 Direction)
{
	// Entryが1つもなければ処理を終了
	if (ItemEntries.Num() == 0)
	{
		return;
	}

	// 選択位置を移動する
	CurrentIndex += Direction;

	// 先頭より前に移動した場合は最後のアイテムへ移動
	if (CurrentIndex < 0)
	{
		CurrentIndex = ItemEntries.Num() - 1;
	}

	// 最後より後に移動した場合は先頭のアイテムへ移動
	else if (CurrentIndex >= ItemEntries.Num())
	{
		CurrentIndex = 0;
	}

	// 移動後の選択状態を更新
	UpdateSelection();

	// Scroll追従
	ItemList->ScrollWidgetIntoView(ItemEntries[CurrentIndex]);
}

void UInventoryWidget::UpdateSelection()
{
	// Entryが1つもなければ処理を終了
	if (ItemEntries.Num() == 0)
	{
		return;
	}

	// すべてのEntryを確認する
	for (int32 i = 0; i < ItemEntries.Num(); i++)
	{
		// 現在選択されているEntryか確認する
		const bool bSelected = (i == CurrentIndex);

		// Entryの選択状態を更新する
		ItemEntries[i]->SetSelected(bSelected);
	}

	// 現在選択されているEntryを取得
	UInventoryItemEntryWidget* SelectedEntry = ItemEntries[CurrentIndex];
	// 選択中のアイテムデータを取得
	const FItemData& ItemData = SelectedEntry->GetItemData();	
	// アイテム名を表示
	ItemNameText->SetText(FText::FromString(ItemData.NameJP));
	// アイテム説明文を表示
	DescriptionText->SetText(FText::FromString(ItemData.Description));
	// 選択中アイテムのアイコンを表示する
	ItemImage->SetBrushFromSoftTexture(ItemData.Icon);
	// 選択されているEntryをScrollBox内に表示する
	ItemList->ScrollWidgetIntoView(SelectedEntry,true,EDescendantScrollDestination::IntoView);
	// 所持金更新
	UpdateMoney();
}

void UInventoryWidget::UpdateMoney()
{
	UInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (MoneyText)
	{
		MoneyText->SetText(FText::FromString(FString::Printf(TEXT("%d G"), Inventory->GetMoney())));
	}
}

void UInventoryWidget::SetupItems()
{
	// 以前の表示をクリア
	ItemEntries.Empty();

	// ScrollBoxが存在しなければ処理を終了
	if (IsValid(ItemList))
	{
		ItemList->ClearChildren();
	}

	// 前回の表示をクリア
	ItemList->ClearChildren();
	// 保持しているEntry配列も空にする
	ItemEntries.Empty();
	// 現在の選択位置を先頭に戻す
	CurrentIndex = 0;

	// InventorySubsystem取得
	UInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
		
	// InventorySubsystemが取得できなければ処理を終了
	if (!Inventory)
	{
		return;
	}

	// GameInstance取得
	UUltoriaGameInstance* GI = GetWorld()->GetGameInstance<UUltoriaGameInstance>();
		
	// GameInstanceが取得できなければ処理を終了
	if (!GI)
	{
		return;
	}

	// ItemDataTable取得
	UDataTable* ItemDataTable = GI->ItemDataTable;

	if (!ItemDataTable)
	{
		return;
	}

	// Inventory取得
	const TArray<FInventoryItem>& InventoryItems = Inventory->GetInventory();
	
	// DataTable検索用のコンテキスト文字列
	static const FString Context(TEXT("Inventory"));

	// Inventoryを1個ずつ処理
	for (const FInventoryItem& InventoryItem : InventoryItems)
	{
		// ItemIDからItemDataを検索
		FItemData* ItemData =ItemDataTable->FindRow<FItemData>(InventoryItem.ItemID,Context);

		if (!ItemData)
		{
			UE_LOG(LogTemp,Error,
				TEXT("ItemData not found : %s"),
				*InventoryItem.ItemID.ToString());
			continue;
		}

		// Entry Widget生成
		if (!InventoryItemEntryClass)
		{
			continue;
		}

		// Entry Widgetを生成
		UInventoryItemEntryWidget* Entry = CreateWidget<UInventoryItemEntryWidget>(GetWorld(), InventoryItemEntryClass);

		// Entryにアイテム情報を渡す
		Entry->SetItemData(*ItemData, InventoryItem.Count);

		// ScrollBoxに追加
		ItemList->AddChild(Entry);

		// 配列にも保存
		ItemEntries.Add(Entry);
	}

	// アイテムがあれば最初を選択
	if (ItemEntries.Num() > 0)
	{
		CurrentIndex = 0;
		UpdateSelection();
	}
}

void UInventoryWidget::OpenItemUseConfirm(const FItemData& ItemData)
{
	// アイテムデータをセット
	PendingItemData = ItemData;
	// 使用確認ウィジェット作成
	ItemUseConfirmWidget = CreateWidget<UItemUseConfirmWidget>(GetWorld(), ItemUseConfirmWidgetClass);
	// アイテム名を設定
	ItemUseConfirmWidget->SetItemName(FText::FromString(ItemData.NameJP));

	ItemUseConfirmWidget->AddToViewport(100);

	bIsConfirmingItemUse = true;
			
}

void UInventoryWidget::ExecutePendingItemUse()
{
	// 確認画面を閉じる
	if (IsValid(ItemUseConfirmWidget))
	{
		ItemUseConfirmWidget->RemoveFromParent();
		ItemUseConfirmWidget = nullptr;
	}

	bIsConfirmingItemUse = false;

	// ItemUseSubsystemを取得
	UItemUseSubsystem* ItemUseSubsystem = GetGameInstance()->GetSubsystem<UItemUseSubsystem>();
		
	// アイテム使用処理
	if (ItemUseSubsystem->UseItem(PendingItemData))
	{
		// 使用成功したら一覧を更新
		SetupItems();
	}

}

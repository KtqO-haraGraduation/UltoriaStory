// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "../../Inventory/UI/InventoryItemEntryWidget.h"
#include "../../Inventory/InventorySubsystem.h"
#include "../../Akiduki/ItemData/ItemData.h"
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
	// 現在選択されているIndexが有効か確認する
	if (!ItemEntries.IsValidIndex(CurrentIndex))
	{
		return;
	}

	// 現在選択されているEntryを取得する
	UInventoryItemEntryWidget* SelectedEntry = ItemEntries[CurrentIndex];

	// Entryが有効か確認する
	if (!IsValid(SelectedEntry))
	{
		return;
	}

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


	// 対象選択と効果適用を追加する
}

void UInventoryWidget::Cancel()
{
	// メニューを管理しているMenuStateManagerを取得する
	UMenuStateManager* MenuManager =
		GetGameInstance()->GetSubsystem<UMenuStateManager>();

	// MenuStateManagerが取得できなければ処理を終了する
	if (!MenuManager)
	{
		return;
	}

	// 現在開いているメニューを閉じる
	MenuManager->CloseMenu();
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
	// 選択Entryが存在しなければ処理を終了
	if (!SelectedEntry)
	{
		return;
	}
		
	// アイテム名を表示
	ItemNameText->SetText(FText::FromString(ItemData.NameJP));
	// アイテム説明文を表示
	DescriptionText->SetText(FText::FromString(ItemData.Description));
	// 選択中アイテムのアイコンを表示する
	ItemImage->SetBrushFromSoftTexture(ItemData.Icon);
	
	// 選択Entryが存在しなければ処理を終了
	if (!SelectedEntry)
	{
		return;
	}

	// 選択されているEntryをScrollBox内に表示する
	ItemList->ScrollWidgetIntoView(SelectedEntry,true,EDescendantScrollDestination::IntoView);
	
}

void UInventoryWidget::SetupItems()
{
	// ScrollBoxが存在しなければ処理を終了
	if (!ItemList)
	{
		return;
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

		// Entryが生成できなければ次のアイテムへ
		if (!Entry)
		{
			continue;
		}

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

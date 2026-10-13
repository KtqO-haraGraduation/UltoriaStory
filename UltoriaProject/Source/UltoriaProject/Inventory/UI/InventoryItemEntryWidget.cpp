// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UInventoryItemEntryWidget::SetItemData(const FItemData& InItemData, int32 InCount)
{
	ItemData = InItemData;// アイテムデータを保存
	Count = InCount;// 所持数を保存

	if (ItemNameText)
	{
		// 名前表示
		ItemNameText->SetText(FText::FromString(ItemData.NameJP));					
	}

	if (ItemCountText)
	{
		// 所持数表示
		ItemCountText->SetText(FText::AsNumber(Count));					
	}
}

void UInventoryItemEntryWidget::SetSelected(bool bSelected)
{
	if (!SelectionBorder) return;// Borderが取得できていなければ処理を終了する

	if (bSelected)
	{
		SelectionBorder->SetBrushColor(FLinearColor::Yellow);// 選択中なら黄色
	}
	else
	{
		SelectionBorder->SetBrushColor(FLinearColor::Transparent);// そうでなければ透明
	}
}
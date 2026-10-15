// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"

void UShopItemEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UShopItemEntryWidget::SetItemData(const FItemData& Data)
{
	// アイテムデータを保存する
	ItemData = Data;

	// アイテム名を表示する
	if (Text_ItemName)
	{
		Text_ItemName->SetText(FText::FromString(ItemData.NameJP));// アイテム名
	}

	// 価格を表示する
	if (Text_Price)
	{
		Text_Price->SetText(FText::AsNumber(ItemData.Price));// 価格
	}

	// アイコンを表示する
	if (Image_ItemIcon)
	{
		// DataTableに設定したアイコンをImageに反映する
		Image_ItemIcon->SetBrushFromSoftTexture(ItemData.Icon);
	}
}

void UShopItemEntryWidget::SetSelected(bool bSelected)
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

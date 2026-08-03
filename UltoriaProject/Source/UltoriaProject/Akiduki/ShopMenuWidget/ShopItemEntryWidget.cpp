// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UShopItemEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UShopItemEntryWidget::SetItemData(const FItemData& Data)
{
	ItemData = Data;

	if (Text_ItemName)
	{
		Text_ItemName->SetText(FText::FromString(ItemData.NameJP));
	}

	if (Text_Price)
	{
		Text_Price->SetText(FText::AsNumber(ItemData.Price));
	}
}

void UShopItemEntryWidget::SetSelected(bool bSelected)
{
	if (!SelectionBorder) return;

	if (bSelected)
	{
		SelectionBorder->SetBrushColor(FLinearColor::Yellow);
	}
	else
	{
		SelectionBorder->SetBrushColor(FLinearColor::Transparent);
	}
}

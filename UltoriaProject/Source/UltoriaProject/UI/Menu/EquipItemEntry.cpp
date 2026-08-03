// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemEntry.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UEquipItemEntry::SetItemName(const FString& Name)
{
	// アイテムの名前があれば設定
	if (m_ItemNameText)
	{
		m_ItemNameText->SetText(FText::FromString(Name));
	}
}

void UEquipItemEntry::SetSelected(bool isSelected)
{
	// 選択中ハイライトがあれば
	if (m_HighlightBorder)
	{
		// 選択中かで表示/非表示
		m_HighlightBorder->SetVisibility(
			isSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden
		);
	}
}

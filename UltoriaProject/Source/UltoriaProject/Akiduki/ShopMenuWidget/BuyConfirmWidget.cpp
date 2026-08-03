// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyConfirmWidget.h"
#include "Components/TextBlock.h"

void UBuyConfirmWidget::SetupItem(const FItemData& InItem, int32 PlayerMoney)
{
	ItemData = InItem;

	BuyCount = 1;

	MaxBuyCount = PlayerMoney / ItemData.Price;
		
	//if (MaxBuyCount < 1)
	//{
	//	MaxBuyCount = 1;
	//}

	RefreshUI();
}

void UBuyConfirmWidget::ChangeCount(int32 Delta)	
{
	BuyCount += Delta;
	BuyCount = FMath::Clamp(BuyCount,1,MaxBuyCount);

	RefreshUI();
}

void UBuyConfirmWidget::RefreshUI()
{
	// ƒAƒCƒeƒ€–¼
	if (ItemNameText)
	{
		ItemNameText->SetText(FText::FromString(ItemData.NameJP));	
	}
	// w“ü”
	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),
			BuyCount,MaxBuyCount)));			
	}
	// ‡Œv‹àŠz
	if (TotalPriceText)
	{
		TotalPriceText->SetText(FText::AsNumber(ItemData.Price * BuyCount));	
	}
}
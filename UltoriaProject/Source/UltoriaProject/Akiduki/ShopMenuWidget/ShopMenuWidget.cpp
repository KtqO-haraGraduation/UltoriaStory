// Fill out your copyright notice in the Description page of Project Settings.
#include "ShopMenuWidget.h"
#include "ShopItemEntryWidget.h"
#include "BuyConfirmWidget.h"
#include "Components/ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"

void UShopMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//if (!ItemDataTable) return;
	//if (!ShopItemEntryClass) return;
	//if (!ItemList) return;

	//// 全行取得
	//static const FString ContextString(TEXT("ItemData"));

	//TArray<FItemData*> Rows;
	//ItemDataTable->GetAllRows<FItemData>(ContextString, Rows);


	//for (FItemData* Row : Rows)
	//{
	//	if (!Row) continue;

	//	// Widget生成	
	//	UShopItemEntryWidget* Entry = CreateWidget<UShopItemEntryWidget>(GetWorld(), ShopItemEntryClass);
	//			
	//	if (Entry)
	//	{
	//		// データセット
	//		Entry->SetItemData(*Row);
	//		// ScrollBox追加
	//		ItemList->AddChild(Entry);
	//		// 配列追加
	//		ItemEntries.Add(Entry);
	//	}
	//}

	//UpdateSelection();
	UpdateMoney();// 所持金更新
}

void UShopMenuWidget::MoveSelection(int32 Direction)
{
	if (ItemEntries.Num() == 0) return;

	CurrentIndex += Direction;

	// ループ
	if (CurrentIndex < 0)
	{
		CurrentIndex = ItemEntries.Num() - 1;
	}

	if (CurrentIndex >= ItemEntries.Num())
	{
		CurrentIndex = 0;
	}

	UpdateSelection();

	// Scroll追従
	ItemList->ScrollWidgetIntoView(ItemEntries[CurrentIndex]);
}

void UShopMenuWidget::UpdateSelection()
{
	for (int32 i = 0; i < ItemEntries.Num(); i++)
	{
		bool bSelected = (i == CurrentIndex);

		ItemEntries[i]->SetSelected(bSelected);
	}

	if (!ItemEntries.IsValidIndex(CurrentIndex))
	{
		return;
	}

	const FItemData& Item = ItemEntries[CurrentIndex]->ItemData;
		
	if (ItemNameText)
	{
		ItemNameText->SetText(FText::FromString(Item.NameJP));
	}

	if (DescriptionText)
	{
		DescriptionText->SetText(FText::FromString(Item.Description));	
	}
}

bool UShopMenuWidget::ConfirmSelection()
{
	if (!ItemEntries.IsValidIndex(CurrentIndex)) return false;
	FItemData Item = ItemEntries[CurrentIndex]->ItemData;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return false;
	UShopManager* Shop = GetWorld()->GetSubsystem<UShopManager>();
	if (!Shop) return false;

	 //所持金不足
	if (Shop->GetMoney() < Item.Price)
	{
		UE_LOG(LogTemp, Warning, TEXT("okanenaiyo"));
		return false;
	}

	// 購入確認生成
	BuyConfirmWidget = CreateWidget<UBuyConfirmWidget>(GetWorld(), BuyConfirmClass);
	if (!BuyConfirmWidget)return false;
	BuyConfirmWidget->AddToViewport();
	BuyConfirmWidget->SetupItem(Item, Shop->GetMoney());

	return true;
	//if (Shop->GetMoney() >= Item.Price)
	//{
	//	BuyConfirmWidget = CreateWidget<UBuyConfirmWidget>(GetWorld(), BuyConfirmClass);

	//	BuyConfirmWidget->AddToViewport();

	//	BuyConfirmWidget->SetupItem(Item,Shop->GetMoney());
	//}

}

void UShopMenuWidget::BuyCurrentItem()
{
	if (!BuyConfirmWidget)
	{
		return;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();		
	if (!PC) return;

	UShopManager* Shop = GetWorld()->GetSubsystem<UShopManager>();
	if (!Shop) return;

	FItemData Item = BuyConfirmWidget->GetItemData();		
	int32 Count = BuyConfirmWidget->GetBuyCount();		
	int32 TotalPrice = Item.Price * Count;
	
	// アイテム購入処理
	if (Shop->GetMoney() >= TotalPrice)
	{
		// 所持金減算
		int nowMoney = Shop->GetMoney();
		nowMoney -= TotalPrice;
		Shop->SetMoney(nowMoney);

		UpdateMoney();// 所持金更新

		UE_LOG(LogTemp,Warning,TEXT("%s x%d Buy nokorinoOkane:%d G"),
			*Item.NameJP,Count, Shop->GetMoney());
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("okanetarinaiyo!!"));
	}

	BuyConfirmWidget->RemoveFromParent();
	BuyConfirmWidget = nullptr;
}

void UShopMenuWidget::UpdateMoney()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
		
	if (!PC) return;

	UShopManager* Shop = GetWorld()->GetSubsystem<UShopManager>();
		
	if (!Shop) return;

	if (MoneyText)
	{
		MoneyText->SetText(FText::FromString(FString::Printf(TEXT("%d G"), Shop->GetMoney())));	
	}
}

void UShopMenuWidget::SetupItems(const TArray<FItemData>& Items)
{
	ItemEntries.Empty();

	ItemList->ClearChildren();

	for (const FItemData& Item : Items)
	{
		UShopItemEntryWidget* Entry =
			CreateWidget<UShopItemEntryWidget>(GetWorld(),ShopItemEntryClass);

		if (!Entry)
			continue;

		Entry->SetItemData(Item);

		ItemList->AddChild(Entry);

		ItemEntries.Add(Entry);
	}

	CurrentIndex = 0;

	UpdateSelection();
}

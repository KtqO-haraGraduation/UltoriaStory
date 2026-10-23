// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUIWidget.h"
#include "ItemButtonWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "../../../Battle/Combat/Data/BattleDataManager.h"
#include "../../../Inventory/InventorySubsystem.h"
#include "../Data/BattleUIDataAsset.h"

void UItemUIWidget::CreateButton()
{
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (!battleDataMg) return;

	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (!gameInstance) return;

	UInventorySubsystem* inventory = gameInstance->GetSubsystem<UInventorySubsystem>();
	if (!inventory) return;

	TSubclassOf<UItemButtonWidget> buttonClass = battleDataMg->GetBattleUIDataAsset()->ItemButtonClass;

	for (int i = 0; i < inventory->GetInventory().Num(); i++)
	{
		UItemButtonWidget* button = 
			CreateWidget<UItemButtonWidget>(GetWorld(), buttonClass);

		if (VB_List) VB_List->AddChild(button);

		// ボタン同士の空いたをあける処理
		if (VB_List)
		{
			UVerticalBoxSlot* slot =
				Cast<UVerticalBoxSlot>(button->Slot);
			m_Buttons.Add(button);

			if (slot)
			{
				slot->SetPadding(FMargin(0.0f, m_Padding, 0.0f, m_Padding));
			}
		}

		// 自身（ボタン）のオーナーとインデックスを登録
		m_Buttons[i]->SetOwner(this, i);
	}
}

void UItemUIWidget::SetButtonData()
{
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (!gameInstance) return;

	UInventorySubsystem* inventory = gameInstance->GetSubsystem<UInventorySubsystem>();
	if (!inventory) return;

	for (int i = 0; i < inventory->GetInventory().Num(); i++)
	{
		const FInventoryItem& itemData = inventory->GetInventory()[i];

		FName itemText = FName(*FString::Printf(TEXT("%s x %d"), *itemData.ItemID.ToString(), itemData.Count));

		m_Buttons[i]->SetMyName(itemText);
	}
}

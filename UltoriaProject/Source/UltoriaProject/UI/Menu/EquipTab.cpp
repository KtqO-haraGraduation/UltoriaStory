// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipTab.h"
#include "../../Equip/EquipmentManager.h"
#include "../../TestPlayer/TestPlayer.h"

void UEquipTab::OnEnter()
{
	// 現在のモードを装備箇所選択に設定
	m_CurrentMode = EEquipTabMode::SlotSelect;
	// 最初のスロットを頭に設定
	m_CurrentSlot = EEquipSlot::Head;
	// 現在のアイテムの順番を始めに設定
	m_CurrentItemIndex = 0;
	// 現在のスロットの所持装備を空に
	m_CurrentSlotItems.Empty();

	// 小窓があれば
	if (m_ItemListPopup)
	{
		// 非表示に
		m_ItemListPopup->SetVisibility(ESlateVisibility::Collapsed);
	}

	UpdateSlotDisplay();
}

void UEquipTab::OnExit()
{
}

void UEquipTab::NavigateUp()
{

	if (m_CurrentMode == EEquipTabMode::SlotSelect)
	{
		int32 slotIndex = (int32)m_CurrentSlot - 1;
		if (slotIndex < 0) slotIndex = 4;
		m_CurrentSlot = (EEquipSlot)slotIndex;
		UpdateSlotDisplay();
	}
	else
	{
		// アイテムリスト上移動
		m_CurrentItemIndex--;
		if (m_CurrentItemIndex < 0)
		{
		    m_CurrentItemIndex = m_CurrentSlotItems.Num() - 1;
		}
		UpdateItemList();
	}
}

void UEquipTab::NavigateDown()
{
	if (m_CurrentMode == EEquipTabMode::SlotSelect)
	{
		return;
	}
	else
	{
		// 次のアイテムへ順番を進める
		m_CurrentItemIndex++;
		// 現在のアイテムの順番が0より下なら
		if (m_CurrentItemIndex >= m_CurrentSlotItems.Num())
		{
			m_CurrentItemIndex = 0;
		}
		// アイテムリストを更新
		UpdateItemList();
	}
}

void UEquipTab::Confirm()
{
	// 装備箇所選択中
	if (m_CurrentMode == EEquipTabMode::SlotSelect)
	{
		// ItemSelectモードに切り替え
		m_CurrentMode = EEquipTabMode::ItemSelect;
		// 現在の選択番号を初期化
		m_CurrentItemIndex = 0;

		// 現在選んでいる装備箇所を取得
		UEquipmentManager* equipManager =
			GetGameInstance()->GetSubsystem<UEquipmentManager>();
		if (equipManager)
		{
			m_CurrentSlotItems = equipManager->GetOwnedItemsBySlot(m_CurrentSlot);
		}

		// 選択中の装備箇所に応じた所持装備リストを反映
		UpdateItemList();
	}
	else
	{
		// 装備確定
		if (!m_CurrentSlotItems.IsValidIndex(m_CurrentItemIndex)) return;

		// 選択している装備をIdから取得
		FName selectedId = m_CurrentSlotItems[m_CurrentItemIndex];
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (!PC) return;

		// プレイヤーの装備を更新
		ATestPlayer* player = Cast<ATestPlayer>(PC->GetPawn());
		if (player)
		{
			player->EquipItem(m_CurrentSlot, selectedId);
		}

		// SlotSelectModeに戻る
		m_CurrentMode = EEquipTabMode::SlotSelect;
		UpdateSlotDisplay();
	}
}

void UEquipTab::Cancel()
{
	if (m_CurrentMode == EEquipTabMode::ItemSelect)
	{
		// SlotSelectモードに戻る
		m_CurrentMode = EEquipTabMode::SlotSelect;
		UpdateSlotDisplay();
	}
}

void UEquipTab::UpdateSlotDisplay()
{
	TArray<UBorder*> frames = {
		m_FrameEquip,m_FrameEquip1,m_FrameEquip2,m_FrameEquip3,m_FrameEquip4
	};

	for (int32 i = 0; i < frames.Num(); i++)
	{
		// 指定したフレームが存在しなければ
		if (!frames[i]) continue;
		// 選択されたフレームを表示
		frames[i]->SetVisibility(
			i == (int32)m_CurrentSlot
			? ESlateVisibility::Visible
			: ESlateVisibility::Hidden
		);
	}

	// 現在装備中の装備名を更新
	TArray<UTextBlock*> nameTexts = {
		m_EquippedNameWeapon,m_EquippedNameHead,m_EquippedNameBody,
		m_EquippedNameWaist,m_EquippedNameFeet
	};

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	ATestPlayer* player = Cast<ATestPlayer>(PC->GetPawn());
	if (!player)return;
	UEquipmentManager* equipManager = GetGameInstance()->GetSubsystem<UEquipmentManager>();
	if (!equipManager)return;

	for (int32 i = 0; i < nameTexts.Num(); i++)
	{
		// 装備名のテキストが無ければ
		if (!nameTexts[i])continue;

		// 装備名を取得
		FName equippedId = player->GetEquippedId((EEquipSlot)i);

		// 装備名が無しになっていれば
		if (equippedId == NAME_None)
		{
			nameTexts[i]->SetText(FText::FromString(TEXT("なし")));
		}
		else
		{
			FEquipItemRow* data = equipManager->GetEquipData(equippedId);
			nameTexts[i]->SetText(FText::FromString(data ? data->ItemName : TEXT("不明")));
		}
	}
}

void UEquipTab::UpdateItemList()
{
	// アイテムスクロールボックスがなければ
	if (!m_ItemScrollBox)return;

	// 一旦クリア
	m_ItemScrollBox->ClearChildren();

	// 現在のモードが装備選択だった場合
	if (m_CurrentMode == EEquipTabMode::ItemSelect)
	{
		// 小窓を表示
		if (m_ItemListPopup)
		{
			m_ItemListPopup->SetVisibility(ESlateVisibility::Visible);
		}

		// アイテムエントリを動的生成
		UEquipmentManager* equipManager = GetGameInstance()->GetSubsystem<UEquipmentManager>();
		if (!equipManager)return;

		// アイテムエントリ動的生成
		for (int32 i = 0; i < m_CurrentSlotItems.Num(); i++)
		{
			// アイテムエントリクラスが無ければ次へ
			if (!m_ItemEntryClass)continue;

			// アイテム名をマスターデータから取得
			FEquipItemRow* data = equipManager->GetEquipData(m_CurrentSlotItems[i]);
			if (!data)continue;

			// アイテムエントリウィジェットを作成
			UEquipItemEntry* entry = CreateWidget<UEquipItemEntry>(
				GetOwningPlayer(), m_ItemEntryClass
			);

			// エントリクラスを作成出来れば
			if (entry)
			{
				// 装備の名前と選択したアイテムを設定する
				entry->SetItemName(data->ItemName);
				entry->SetSelected(i == m_CurrentItemIndex);
				m_ItemScrollBox->AddChild(entry);
			}
		}
	}
	else
	{
		// 小窓を非表示
		if (m_ItemListPopup)
		{
			m_ItemListPopup->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandUIWidget.h"
#include "CommandButtonWidget.h"
#include "CommandCursorWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "Data/BattleUIDataAsset.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"

void UCommandUIWidget::CreateButton()
{
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();

	// コマンドボタンのクラスを取得
	TSubclassOf<UCommandButtonWidget> buttonClass = battleDataMg->GetBattleUIDataAsset()->CommandButtonClass;

	// 登録されたリスト分のコマンドButtonを生成
	for (auto& command : battleDataMg->GetCommandDatas())
	{
		UCommandButtonWidget* button =
			CreateWidget<UCommandButtonWidget>(GetWorld(), buttonClass);

		if (button) button->SetCommandData(command);
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
	}

	// 自身（ボタン）のオーナーとインデックスを登録
	for (int i = 0; i < m_Buttons.Num(); i++)
	{
		m_Buttons[i]->SetOwner(this, i);
	}
}

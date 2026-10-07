// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandUIWidget.h"
#include "CommandButtonWidget.h"
#include "CommandCursorWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"

void UCommandUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// コマンドのボタンを生成
	UBattleDataManager* dataManager = GetWorld()->GetSubsystem<UBattleDataManager>();

	dataManager->LoadCommandData();

	// コマンドボタンのクラスを取得
	UClass* buttonClass =
		LoadClass<UCommandButtonWidget>(
			nullptr, 
			TEXT("/Game/MembarFiles/Matsubara/UI_MC/Command/WBP_CommandButton.WBP_CommandButton_C"));
	
	// 登録されたリスト分のコマンドButtonを生成
	for (auto& command : dataManager->GetCommandDatas())
	{
		UCommandButtonWidget* button =
			CreateWidget<UCommandButtonWidget>(GetWorld(), buttonClass);

		if(button) button->SetCommandData(command);
		if(VB_List) VB_List->AddChild(button);

		// ボタン同士の空いたをあける処理
		if (VB_List)
		{
			UVerticalBoxSlot* slot =
				Cast<UVerticalBoxSlot>(button->Slot);
			m_Buttons.Add(button);

			if (slot)
			{
				slot->SetPadding(FMargin(0.0f, CharacterPadding, 0.0f, CharacterPadding));
			}
		}
	}

	// 自身（ボタン）のオーナーとインデックスを登録
	for (int i = 0; i < m_Buttons.Num(); i++)
	{
		m_Buttons[i]->SetOwner(this, i);
	}
}

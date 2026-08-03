// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandUIWidget.h"
#include "CommandButtonWidget.h"
#include "CommandCursorWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"

constexpr int INIT_COUNT_MAX = 1;

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
		if(VB_CommandList) VB_CommandList->AddChild(button);

		// ボタン同士の空いたをあける処理
		if (VB_CommandList)
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

void UCommandUIWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	// 指定したフレーム分待機
	if (m_InitCount < INIT_COUNT_MAX)
	{
		m_InitCount++;
		return;
	}

	Super::NativeTick(MyGeometry, DeltaTime);

	// 1度だけ実行
	// カーソルの選択初期化
	if (!m_IsInitialized && m_InitCount >= INIT_COUNT_MAX)
	{
		WBP_CommandCursor->SetFirst(!m_IsInitialized);
		UpdateSelection(0);
		m_IsInitialized = true;
	}
}

void UCommandUIWidget::UpdateSelection(int index)
{
	for (int i = 0; i < m_Buttons.Num(); i++)
	{
		m_Buttons[i]->SetSelected(i == index);
	}

	if (m_Buttons.IsValidIndex(index) && WBP_CommandCursor)
	{
		// 新しい位置へ移動
		FVector2D pos = GetCursorPos(m_Buttons[index]);
		FVector2D offset = { -50.0f, -13.0f };
		pos += offset;

		WBP_CommandCursor->MoveTo(pos);
	}

	m_CurrentIndex = index;
}

FVector2D UCommandUIWidget::GetCursorPos(UCommandButtonWidget* button) const
{
	if (!button || !CanvasPanel)
	{
		return FVector2D::ZeroVector;
	}

	FVector2D absolutePos =
		button->GetCachedGeometry().GetAbsolutePosition();

	FVector2D localPos =
		CanvasPanel->GetCachedGeometry().AbsoluteToLocal(absolutePos);

	return localPos;
}

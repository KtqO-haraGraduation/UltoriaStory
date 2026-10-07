// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleSelectionUIBase.h"
#include "BattleButtonBase.h"
#include "../CommandCursorWidget.h"

constexpr int INIT_COUNT_MAX = 10;					// 初期化の待機カウント
const FVector2D CURSOR_OFFSET = { -50.0f, -13.0f };	// カーソルの間隔

void UBattleSelectionUIBase::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
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
		WBP_Cursor->SetFirst(!m_IsInitialized);
		UpdateSelection(0);
		m_IsInitialized = true;
	}
}

void UBattleSelectionUIBase::UpdateSelection(int index)
{
	for (int i = 0; i < m_Buttons.Num(); i++)
	{
		m_Buttons[i]->SetSelected(i == index);
	}

	if (m_Buttons.IsValidIndex(index) && WBP_Cursor)
	{
		// 新しい位置へ移動
		FVector2D pos = GetCursorPos(m_Buttons[index]);
		pos += CURSOR_OFFSET;

		WBP_Cursor->MoveTo(pos);
	}

	m_CurrentIndex = index;
}

FVector2D UBattleSelectionUIBase::GetCursorPos(UBattleButtonBase* button) const
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

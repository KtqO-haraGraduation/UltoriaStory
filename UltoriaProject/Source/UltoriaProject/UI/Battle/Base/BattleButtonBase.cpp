// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleButtonBase.h"
#include "BattleSelectionUIBase.h"

void UBattleButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	// ボタンの設定
	if (Button)
	{
		// 選択した時の処理
		Button->OnClicked.AddUniqueDynamic(
			this,
			&UBattleButtonBase::OnClickButton
		);

		// 選んでるときの処理
		Button->OnHovered.AddUniqueDynamic(
			this,
			&UBattleButtonBase::OnHoverButton
		);
	}
}

void UBattleButtonBase::SetOwner(UBattleSelectionUIBase* owner, int index)
{
	if (!owner) return;

	m_Owner = owner;
	m_Index = index;
}

void UBattleButtonBase::SetSelected(bool flag)
{
	if (!Button || m_IsSelected == flag) return;

	m_IsSelected = flag;

	// ボタンの背景色
	FLinearColor color = flag ? FLinearColor::Yellow : FLinearColor::White;
	Button->SetBackgroundColor(color);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandButtonWidget.h"
#include "CommandUIWidget.h"
#include "../../Battle/BattleManager.h"

void UCommandButtonWidget::SetCommandData(const FBattleCommandData& data)
{
	// コマンドデータを設定
	m_CommandData = data;

	// 名前設定
	if (TB_Name)
	{
		TB_Name->SetText(FText::FromName(data.Name));
	}
}

void UCommandButtonWidget::SetSelected(bool flag)
{
	if (!Button || m_IsSelected == flag) return;

	UBattleButtonBase::SetSelected(flag);

	if (flag)
	{
		PlayAnimation(SelectAnim);
	}
	else
	{
		PlayAnimationReverse(SelectAnim);
	}
}

void UCommandButtonWidget::OnClickButton()
{
	UBattleManager* battleManager = 
		GetWorld()->GetSubsystem<UBattleManager>();

	if (!battleManager) return;

	// バトルマネージャーに通知
	battleManager->ExecuteCommand(m_CommandData.Type);
}

void UCommandButtonWidget::OnHoverButton()
{
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();

	if (m_Owner)
	{
		m_Owner->UpdateSelection(m_Index);
		if (battleManager) battleManager->SetCommandSelectIndex(m_Index);
	}
}

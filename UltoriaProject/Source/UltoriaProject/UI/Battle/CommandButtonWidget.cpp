// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandButtonWidget.h"
#include "CommandUIWidget.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/Combat/Selection/BattleSelectioinManager.h"

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
		if (battleManager) battleManager->GetSelectManager()->SetCommandSelectIndex(m_Index);
	}
}

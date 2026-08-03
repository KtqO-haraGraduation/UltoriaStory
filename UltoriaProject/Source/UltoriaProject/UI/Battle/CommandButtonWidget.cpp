// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandButtonWidget.h"
#include "CommandUIWidget.h"
#include "../../Battle/BattleManager.h"

void UCommandButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ボタンが押された処理を登録
	if (Button)
	{
		SetIsFocusable(false);

		Button->OnClicked.AddUniqueDynamic(
			this,
			&UCommandButtonWidget::OnClickCommandButton);

		Button->OnHovered.AddUniqueDynamic(
			this,
			&UCommandButtonWidget::OnHoverCommandButton);
	}
}

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

void UCommandButtonWidget::SetOwner(UCommandUIWidget* owner, int index)
{
	if (!owner) return;

	m_Owner = owner;
	m_Index = index;
}

void UCommandButtonWidget::SetSelected(bool flag)
{
	if (!Button || m_IsSelected == flag) return;

	m_IsSelected = flag;

	// 選択時：黄色、通常：白
	FLinearColor color = flag ? FLinearColor::Yellow : FLinearColor::White;
	Button->SetBackgroundColor(color);

	if (flag)
	{
		PlayAnimation(SelectAnim);
	}
	else
	{
		PlayAnimationReverse(SelectAnim);
	}
}

void UCommandButtonWidget::OnClickCommandButton()
{
	UBattleManager* battleManager = 
		GetWorld()->GetSubsystem<UBattleManager>();

	if (!battleManager) return;

	// バトルマネージャーに通知
	battleManager->ExecuteCommand(m_CommandData.Type);
}

void UCommandButtonWidget::OnHoverCommandButton()
{
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();

	if (m_Owner)
	{
		m_Owner->UpdateSelection(m_Index);
		if (battleManager) battleManager->SetCommandSelectIndex(m_Index);
	}
}

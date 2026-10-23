// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemButtonWidget.h"
#include "ItemUIWidget.h"
#include "../../../Battle/BattleManager.h"
#include "../../../Battle/Combat/Components/CombatComponent.h"
#include "../../../Battle/Combat/Selection/BattleSelectioinManager.h"
#include "../../../Job/Skill/SkillBaseComponent.h"
#include "../../../Character/CharacterBase.h"

void UItemButtonWidget::OnClickButton()
{

}

void UItemButtonWidget::OnHoverButton()
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();

	if (m_Owner)
	{
		m_Owner->UpdateSelection(m_Index);
		if (battleMg) battleMg->GetSelectManager()->SetItemSelectIndex(m_Index);
	}
}

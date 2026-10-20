// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillButtonWidget.h"
#include "SkillUIWidget.h"
#include "../../../Battle/BattleManager.h"
#include "../../../Battle/Combat/Components/CombatComponent.h"
#include "../../../Battle/Combat/Selection/BattleSelectioinManager.h"
#include "../../../Job/Skill/SkillBaseComponent.h"
#include "../../../Character/CharacterBase.h"

void USkillButtonWidget::OnClickButton()
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	ACharacterBase* currentCharacter = battleMg->GetCurrentCharacter();
	if (!currentCharacter) return;

	UCombatComponent* conbat = currentCharacter->GetComponent<UCombatComponent>();
	if (!conbat) return;

	USkillBaseComponent* skill = currentCharacter->GetComponent<USkillBaseComponent>();
	if (!skill) return;


}

void USkillButtonWidget::OnHoverButton()
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();

	if (m_Owner)
	{
		m_Owner->UpdateSelection(m_Index);
		if (battleMg) battleMg->GetSelectManager()->SetSkillSelectIndex(m_Index);
	}
}

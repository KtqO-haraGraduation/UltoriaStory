// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillButtonWidget.h"
#include "../../../Battle/BattleManager.h"
#include "../../../Battle/Combat/Components/CombatComponent.h"
#include "../../../Character/CharacterBase.h"

void USkillButtonWidget::SetSkillData(FName skillName)
{
	if (TB_Name)
	{
		TB_Name->SetText(FText::FromName(skillName));
	}
}

void USkillButtonWidget::OnClickButton()
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	ACharacterBase* character = battleMg->GetCurrentCharacter();
	if (!character) return;

	UCombatComponent* conbat = character->GetComponent<UCombatComponent>();
	if (!conbat) return;

	//conbat->Skill();
}

void USkillButtonWidget::OnHoverButton()
{

}

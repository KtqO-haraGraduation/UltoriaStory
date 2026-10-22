// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUIWidget.h"
#include "SkillButtonWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "../CommandCursorWidget.h"
#include "../Data/BattleUIDataAsset.h"
#include "../../../Battle/Combat/Data/BattleDataManager.h"
#include "../../../Battle/Combat/Turn/BattleTurnManager.h"
#include "../../../Battle/BattleManager.h"
#include "../../../Job/Skill/SkillParameter.h"
#include "../../../Job/Skill/SkillBaseComponent.h"
#include "../../../Character/CharacterBase.h"

void USkillUIWidget::CreateButton()
{
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();

	// コマンドボタンのクラスを取得
	TSubclassOf<USkillButtonWidget> buttonClass = battleDataMg->GetBattleUIDataAsset()->SkillButtonClass;

	// Skillボタンを最大値生成
	for (int i = 0; i < SKILL_MAX; i++)
	{
		USkillButtonWidget* button =
			CreateWidget<USkillButtonWidget>(GetWorld(), buttonClass);

		if (VB_List) VB_List->AddChild(button);

		// ボタン同士の空いたをあける処理
		if (VB_List)
		{
			UVerticalBoxSlot* slot =
				Cast<UVerticalBoxSlot>(button->Slot);
			m_Buttons.Add(button);

			if (slot)
			{
				slot->SetPadding(FMargin(0.0f, m_Padding, 0.0f, m_Padding));
			}
		}

		// 自身（ボタン）のオーナーとインデックスを登録
		m_Buttons[i]->SetOwner(this, i);
	}
}

void USkillUIWidget::SetButtonData()
{
	if (!GetCurrentSkillComponent()) return;

	// 全てのスキルボタンを現在行動しているキャラクターのモノに差し替える
	for (int i = 0; i < SKILL_MAX; i++)
	{
		const FSkillDataRow* skillData = GetCurrentSkillComponent()->GetSkill(i);
		if (!skillData) continue;

		m_Buttons[i]->SetMyName(skillData->Name);

		// 解放Lvに達していなければ非表示
		if (!GetCurrentSkillComponent()->IsUnlocked(i))
		{
			m_Buttons[i]->SetVisible(false);
		}
	}
}

void USkillUIWidget::UpdateSelection(int index)
{
	UBattleSelectionUIBase::UpdateSelection(index);

	const FSkillDataRow* skillData = GetCurrentSkillComponent()->GetSkill(m_CurrentIndex);
	if (!skillData) return;

	DescText->SetText(skillData->Desc);
}

USkillBaseComponent* USkillUIWidget::GetCurrentSkillComponent()
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return nullptr;

	ACharacterBase* currentCharacter = battleMg->GetCurrentCharacter();
	if (!currentCharacter) return nullptr;

	return currentCharacter->GetComponent<USkillBaseComponent>();
}

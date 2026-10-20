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

void USkillUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// スキルのボタンを生成
	CreateButton();
}

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

void USkillUIWidget::SetSkillButtonData()
{
	// 全てのスキルボタンを現在行動しているキャラクターのモノに差し替える
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	ACharacterBase* currentCharacter = battleMg->GetCurrentCharacter();
	if (!currentCharacter) return;

	USkillBaseComponent* skillComponent = currentCharacter->GetComponent<USkillBaseComponent>();
	if (!skillComponent) return;

	for (int i = 0; i < SKILL_MAX; i++)
	{
		const FSkillDataRow* skillData = skillComponent->GetSkill(i);
		m_Buttons[i]->SetMyName(skillData->Name);

		// 解放Lvに達していなければ非表示
		if (!skillComponent->IsUnlocked(i))
		{
			m_Buttons[i]->SetVisible(false);
		}
	}
}

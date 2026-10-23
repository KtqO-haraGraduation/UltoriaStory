// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUIWidget.h"
#include "CharacterStatusWidget.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Character/CharacterBase.h"
#include "Base/BattleSelectionUIBase.h"
#include "BattleUIManager.h"


void UBattleUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// プレイヤーのパーティーを参照して、ステータスのUIを生成
	UPlayerPartyManager* partyManager = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (!partyManager) return;

	// プレイヤーのパーティーを取得
	const TArray<ACharacterBase*>& party = partyManager->GetPlayerParty();

	// ステータスをスロットに設定
	for (int i = 0; i < HB_CharacterStatus->GetChildrenCount(); i++)
	{
		// スロットを取得
		UCharacterStatusWidget* status =
			Cast<UCharacterStatusWidget>(HB_CharacterStatus->GetChildAt(i));

		if (!status) continue;

		// キャラクターが存在すれば、ステータスを設定
		if (i < party.Num())
		{
			status->SetCharacter(party[i]);
		}
		// キャラクターがいなければ、非表示
		else
		{
			status->HideUI();
		}
	}
}

UBattleSelectionUIBase* UBattleUIWidget::GetSelectionUI(EBattleUIType type) const
{
	switch (type)
	{
	case EBattleUIType::COMMAND:
		return WBP_Command;

	case EBattleUIType::SKILL:
		return WBP_Skill;

	case EBattleUIType::ITEM:
		return WBP_Item;
	default:
		return nullptr;
	}
}

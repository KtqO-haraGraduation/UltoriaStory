// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUIWidget.h"
#include "CharacterStatusWidget.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Character/CharacterBase.h"

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

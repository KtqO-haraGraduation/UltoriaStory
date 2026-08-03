// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleResultWidget.h"
#include "ResultCharacterWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Battle/BattleManager.h"

void UBattleResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// キャラクターのステータスUIを生成
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();
	UPlayerPartyManager* playerParty = GetWorld()->GetSubsystem<UPlayerPartyManager>();

	// ステータスUIのクラスを取得
	UClass* resultClass =
		LoadClass<UResultCharacterWidget>(
			nullptr,
			TEXT("/Game/MembarFiles/Matsubara/UI_MC/Result/WBP_ResultCharacterWidget.WBP_ResultCharacterWidget_C"));

	// ステータスUIを生成
	for (auto character : playerParty->GetPlayerParty())
	{
		UResultCharacterWidget* result =
			CreateWidget<UResultCharacterWidget>(GetWorld(), resultClass);

		if (result) result->SetCharacter(character);
		if (VB_Character) VB_Character->AddChild(result);

		// ステータスUI同士の間をあける
		if (VB_Character)
		{
			UVerticalBoxSlot* slot =
				Cast<UVerticalBoxSlot>(result->Slot);

			if (slot)
			{
				slot->SetPadding(FMargin(0.0f, ResultPadding, 0.0f, ResultPadding));
			}
		}
	}
}

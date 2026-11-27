// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleResultWidget.h"
#include "ResultCharacterWidget.h"
#include "Data/BattleUIDataAsset.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"
#include "Components/VerticalBoxSlot.h"

void UBattleResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// キャラクターのステータスUIを生成
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	UPlayerPartyManager* playerParty = GetWorld()->GetSubsystem<UPlayerPartyManager>();

	// ステータスUIのクラスを取得
	TSubclassOf<UResultCharacterWidget> resultClass = battleDataMg->GetBattleUIDataAsset()->ResultCharacterWidgetClass;

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

void UBattleResultWidget::SetGoldText(int gold)
{
}

void UBattleResultWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

// Fill out your copyright notice in the Description page of Project Settings.

#include "RaidBattleUIWidget.h"
#include "RaidCommandUIWidget.h"
#include "RaidSpeedUIWidget.h"
#include "../CharacterStatusWidget.h"
#include "../../../Character/CharacterBase.h"
#include "../../../Battle/Raid/RaidBattleManager.h"
#include "../../../Battle/Raid/RaidBattleTypes.h"
#include "Components/HorizontalBox.h"
#include "GameFramework/PlayerController.h"

void URaidBattleUIWidget::SetRaidManager(ARaidBattleManager* manager)
{
	m_RaidManager = manager;
}

void URaidBattleUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ボタンを押せるように、マウスカーソルを出して、ゲームとUIの両方の入力を受ける
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
	}

	if (m_RaidManager)
	{
		m_RaidManager->OnRaidBattleChanged.AddUniqueDynamic(this, &URaidBattleUIWidget::RefreshRaidUI);
		RefreshRaidUI();
	}
}

void URaidBattleUIWidget::NativeDestruct()
{
	if (m_RaidManager)
	{
		m_RaidManager->OnRaidBattleChanged.RemoveDynamic(this, &URaidBattleUIWidget::RefreshRaidUI);
	}
	Super::NativeDestruct();
}

void URaidBattleUIWidget::RefreshRaidUI()
{
	if (!m_RaidManager)return;

	if (HB_CharacterStatus)
	{
		const int32 slotCount = HB_CharacterStatus->GetChildrenCount();
		int32 slotIndex = 0;

		for (const FRaidCharacterState& state : m_RaidManager->GetRaidCharacterStates())
		{
			if (state.bIsBoss || !IsValid(state.Character))continue;
			if (slotIndex >= slotCount)break;

			UCharacterStatusWidget* status = Cast<UCharacterStatusWidget>(HB_CharacterStatus->GetChildAt(slotIndex++));
			if (!status) continue;

			if (status->GetCharacter() != state.Character)
			{
				status->SetCharacter(state.Character);
			}
		}

		for (; slotIndex < slotCount; ++slotIndex)
		{
			if (UCharacterStatusWidget* status = Cast<UCharacterStatusWidget>(HB_CharacterStatus->GetChildAt(slotIndex)))
			{
				status->HideUI();
			}
		}
	}

	ACharacterBase* current = m_RaidManager->GetCurrentCharacter();
	if (m_LastCurrent.Get() != current)
	{
		SetTurnHightlight(m_LastCurrent.Get(), false);
		SetTurnHightlight(current, true);
		m_LastCurrent = current;
	}

	if (WBP_Speed)
	{
		WBP_Speed->RefreshSpeedWidgets(m_RaidManager->GetTurnOrder());
	}

	if (WBP_Command)
	{
		WBP_Command->SetVisible(m_RaidManager->IsMyTurn(GetOwningPlayer()));
	}
}

void URaidBattleUIWidget::SetTurnHightlight(ACharacterBase* character, bool bOn)
{
	if (!character || !HB_CharacterStatus) return;

	for (int32 i = 0; i < HB_CharacterStatus->GetChildrenCount(); ++i)
	{
		UCharacterStatusWidget* status = Cast<UCharacterStatusWidget>(HB_CharacterStatus->GetChildAt(i));
		if (!status || status->GetCharacter() != character) continue;

		if (bOn) status->PlayTurnSelect();
		else status->ReverseTurnSelect();
		return;
	}
}

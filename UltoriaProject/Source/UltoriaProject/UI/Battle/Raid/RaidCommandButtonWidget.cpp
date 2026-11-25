// Fill out your copyright notice in the Description page of Project Settings.

#include "RaidCommandButtonWidget.h"
#include "../Base/BattleSelectionUIBase.h"
#include "../../../Battle/Raid/RaidBattleManager.h"
#include "../../../Battle/Raid/RaidBattleCommandProxy.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"

void URaidCommandButtonWidget::SetRaidCommand(ERaidCommandType command, const FText& label)
{
	m_Command = command;

	if (TB_Name)
	{
		TB_Name->SetText(label);
	}
}

void URaidCommandButtonWidget::OnClickButton()
{
	UWorld* world = GetWorld();
	APlayerController* pc = GetOwningPlayer();
	if (!world || !pc)return;

	for (TActorIterator<ARaidBattleManager> it(world); it; ++it)
	{
		// Ž©•ª‚Ìƒ^[ƒ“‚Å‚È‚¯‚ê‚Î‰½‚à‚µ‚È‚¢
		if (!it->IsMyTurn(pc))return;

		if (ARaidBattleCommandProxy* proxy = it->GetMyCommandProxy(pc))
		{
			proxy->SubmitCommand(m_Command);
		}
		return;
	}
}

void URaidCommandButtonWidget::OnHoverButton()
{
	if (m_Owner)
	{
		m_Owner->UpdateSelection(m_Index);
	}
}

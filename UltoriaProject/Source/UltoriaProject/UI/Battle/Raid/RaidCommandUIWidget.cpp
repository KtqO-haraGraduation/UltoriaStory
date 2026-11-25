// Fill out your copyright notice in the Description page of Project Settings.

#include "RaidCommandUIWidget.h"
#include "RaidCommandButtonWidget.h"
#include "Components/VerticalBoxSlot.h"

URaidCommandUIWidget::URaidCommandUIWidget(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	FRaidCommandEntry attack;
	attack.Command = ERaidCommandType::ATTACK;

	FRaidCommandEntry guard;
	guard.Command = ERaidCommandType::GUARD;

	m_CommandEntries = { attack, guard };
}

void URaidCommandUIWidget::CreateButton()
{
	if (!m_ButtonClass || !VB_List)
	{
		return;
	}

	const UEnum* commandEnum = StaticEnum<ERaidCommandType>();

	for (const FRaidCommandEntry& entry : m_CommandEntries)
	{
		URaidCommandButtonWidget* button = CreateWidget<URaidCommandButtonWidget>(this, m_ButtonClass);
		if (!button)continue;

		FText label = entry.Label;
		if (label.IsEmpty() && commandEnum)
		{
			label = commandEnum->GetDisplayNameTextByValue(static_cast<int64>(entry.Command));
		}
		button->SetRaidCommand(entry.Command, label);

		VB_List->AddChild(button);

		if (UVerticalBoxSlot* boxSlot = Cast<UVerticalBoxSlot>(button->Slot))
		{
			boxSlot->SetPadding(FMargin(0.0f, m_Padding, 0.0f, m_Padding));
		}

		m_Buttons.Add(button);
	}

	for (int32 i = 0; i < m_Buttons.Num(); ++i)
	{
		m_Buttons[i]->SetOwner(this, i);
	}
}

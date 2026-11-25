// Fill out your copyright notice in the Description page of Project Settings.

#include "RaidSpeedUIWidget.h"
#include "../Speed/SpeedWidget.h"
#include "../../../Character/CharacterBase.h"
#include "Components/CanvasPanelSlot.h"

void URaidSpeedUIWidget::RefreshSpeedWidgets(const TArray<TObjectPtr<ACharacterBase>>& turnOrder)
{
	if (!SpeedPanel || !SpeedWidgetClass)return;

	TArray<ACharacterBase*> characters;
	for (ACharacterBase* character : turnOrder)
	{
		if (IsValid(character))
		{
			characters.Add(character);
		}
	}

	if (characters.Num() == m_Shown.Num())
	{
		bool bSame = true;
		for (int32 i = 0; i < characters.Num(); ++i)
		{
			if (m_Shown[i].Get() != characters[i])
			{
				bSame = false;
				break;
			}
		}
		if (bSame)return;
	}

	for (USpeedWidget* widget : m_Widgets)
	{
		if (widget)widget->RemoveFromParent();
	}
	m_Widgets.Reset();
	m_Shown.Reset();

	int32 index = 0;
	for (ACharacterBase* character : characters)
	{
		USpeedWidget* widget = CreateWidget<USpeedWidget>(this, SpeedWidgetClass);
		if (!widget)continue;

		widget->SetCharacter(character);

		UCanvasPanelSlot* panelSlot = SpeedPanel->AddChildToCanvas(widget);
		if (!panelSlot)continue;

		panelSlot->SetPosition(FVector2D(StartX + index * (ItemWidth + Spacing), StartY));

		m_Widgets.Add(widget);
		m_Shown.Add(character);
		++index;
	}
}

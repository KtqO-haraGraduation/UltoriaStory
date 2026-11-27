// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveWidget.h"
#include "Components/TextBlock.h"

void UObjectiveWidget::SetObjective(const FText& text)
{
	if (m_ObjectiveText)
	{
		m_ObjectiveText->SetText(text);
	}

	// –Ú“I‚ª‚È‚¢‚Æ‚«‚Í•\Ž¦‚µ‚È‚¢
	SetVisibility(text.IsEmpty() ? ESlateVisibility::Collapsed
		                         : ESlateVisibility::HitTestInvisible);
}

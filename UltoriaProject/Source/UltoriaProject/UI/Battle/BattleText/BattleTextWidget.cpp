// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTextWidget.h"

void UBattleTextWidget::SetItemText(FText text)
{
	BattleText->SetText(text);
}

void UBattleTextWidget::SetSkillText(FText text)
{
	BattleText->SetText(text);
}

void UBattleTextWidget::ResetText()
{
	BattleText->SetText(FText::GetEmpty());
}

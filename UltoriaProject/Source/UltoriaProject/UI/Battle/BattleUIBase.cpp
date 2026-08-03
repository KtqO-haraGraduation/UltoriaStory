// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleUIBase.h"

void UBattleUIBase::BattleStart()
{
	ShowUI();
}

void UBattleUIBase::BattleEnd()
{
	HideUI();
}

void UBattleUIBase::SetVisible(bool flag)
{
	flag ? ShowUI() : HideUI();
}

void UBattleUIBase::ShowUI()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UBattleUIBase::HideUI()
{
	SetVisibility(ESlateVisibility::Hidden);
}

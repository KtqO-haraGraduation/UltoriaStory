// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBase.h"

void UMenuBase::Show()
{
		SetVisibility(ESlateVisibility::Visible);
		OnEnter();
}

void UMenuBase::Hide()
{
		SetVisibility(ESlateVisibility::Hidden);
		OnExit();
}


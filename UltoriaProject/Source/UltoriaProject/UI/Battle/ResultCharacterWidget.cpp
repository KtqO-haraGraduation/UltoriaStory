// Fill out your copyright notice in the Description page of Project Settings.

#include "ResultCharacterWidget.h"
#include "../../Character/CharacterBase.h"

void UResultCharacterWidget::SetCharacter(ACharacterBase* character)
{
	m_Character = character;

	if (!m_Character)
	{
		HideUI();
		return;
	}

	ShowUI();

	SetImage();
	SetNameText();
	SetLvText();
	SetNextLvText();
}

void UResultCharacterWidget::SetImage()
{

}

void UResultCharacterWidget::SetNameText()
{
	NameText->SetText(
		FText::FromString(m_Character->GetCharacterName()));
}

void UResultCharacterWidget::SetLvText()
{
	if (!LvText) return;

	LvText->SetText(
		FText::Format(
			FText::FromString(TEXT("Lv {0}")),
			m_Character->GetLevel()
		)
	);
}

void UResultCharacterWidget::SetNextLvText()
{
	if (!NextLvText) return;

	NextLvText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}")),
			m_Character->GetLevel()
		)
	);
}

void UResultCharacterWidget::UpdateLvBar()
{

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../../Character/CharacterBase.h"

void UCharacterStatusWidget::SetCharacter(ACharacterBase* character)
{
	if (m_Character)
	{
		m_Character->GetOnHPChanged().RemoveAll(this);
		m_Character->GetOnMPChanged().RemoveAll(this);
		m_Character = nullptr;
	}

	m_Character = character;

	if (!m_Character)
	{
		HideUI();
		return;
	}

	m_Character->GetOnHPChanged().AddUniqueDynamic(
		this,
		&UCharacterStatusWidget::UpdateHP
	);

	m_Character->GetOnMPChanged().AddUniqueDynamic(
		this,
		&UCharacterStatusWidget::UpdateMP
	);

	// UI‚ð•\Ž¦
	ShowUI();

	SetNameText();
	UpdateHP();
	UpdateMP();
}

void UCharacterStatusWidget::SetNameText()
{
	NameText->SetText(
		FText::FromString(m_Character->GetCharacterName()));
}

void UCharacterStatusWidget::UpdateHP()
{
	if (!HPText || !HPBar) return;

	HPText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}/{1}")),
			m_Character->GetCurrentHP(),
			m_Character->GetMaxHP()
		)
	);

	HPBar->SetPercent(m_Character->GetHPRate());
}

void UCharacterStatusWidget::UpdateMP()
{
	if (!MPText || !MPBar) return;

	MPText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}/{1}")),
			m_Character->GetCurrentMP(),
			m_Character->GetMaxMP()
		)
	);

	MPBar->SetPercent(m_Character->GetMPRate());
}

void UCharacterStatusWidget::HideMPUI()
{
	if (MPText)
	{
		MPText->SetVisibility(ESlateVisibility::Hidden);
	}
	if (MPBar)
	{
		MPBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCharacterStatusWidget::PlayTurnSelect()
{
	if (!TurnSelectAnim) return;
	PlayAnimation(TurnSelectAnim);
}

void UCharacterStatusWidget::ReverseTurnSelect()
{
	if (!TurnSelectAnim) return;
	PlayAnimationReverse(TurnSelectAnim);
}

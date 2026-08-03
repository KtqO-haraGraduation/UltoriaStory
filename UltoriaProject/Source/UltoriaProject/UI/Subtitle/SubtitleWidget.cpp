// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget.h"
#include "Components/TextBlock.h"

void USubtitleWidget::SetSubtitle(const FString& SpeakerName, const FString& Text)
{
	// –¼‘O‚ª‚ ‚ê‚Î
	if (m_SpeakerNameText)
	{
		m_SpeakerNameText->SetText(FText::FromString(SpeakerName));
	}

	m_FullBodyText = Text;
	m_DisplayBodyText.Empty();
	m_CurrentLetterIndex = 0;
	m_IsTyping = true;

	if (m_SubtitleBodyText)
	{
		m_SubtitleBodyText->SetText(FText::GetEmpty());
	}

	GetWorld()->GetTimerManager().SetTimer(m_TypingTimerHandle, this, &USubtitleWidget::TypeNextLetter, 0.05f, true);
}

void USubtitleWidget::HideSubtitle()
{
	GetWorld()->GetTimerManager().ClearTimer(m_TypingTimerHandle);
	m_IsTyping = false;
	SetVisibility(ESlateVisibility::Hidden);
}

void USubtitleWidget::TypeNextLetter()
{
	if (m_CurrentLetterIndex >= m_FullBodyText.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(m_TypingTimerHandle);
		m_IsTyping = false;
		return;
	}

	m_DisplayBodyText.AppendChar(m_FullBodyText[m_CurrentLetterIndex]);
	m_CurrentLetterIndex++;

	if (m_SubtitleBodyText)
	{
		m_SubtitleBodyText->SetText(FText::FromString(m_DisplayBodyText));
	}
}

void USubtitleWidget::SkipTyping()
{
	GetWorld()->GetTimerManager().ClearTimer(m_TypingTimerHandle);

	m_DisplayBodyText = m_FullBodyText;
	m_CurrentLetterIndex = m_FullBodyText.Len();
	m_IsTyping = false;

	if (m_SubtitleBodyText)
	{
		m_SubtitleBodyText->SetText(FText::FromString(m_DisplayBodyText));
	}
}

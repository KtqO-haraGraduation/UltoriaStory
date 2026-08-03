// Fill out your copyright notice in the Description page of Project Settings.


#include "DIalougeWidget.h"

void UDIalougeWidget::SetSpeakerText(const FString& text)
{
	// もし話者の名前があれば
	if (m_SpeakerText)
	{
		// セリフを設定
		m_SpeakerText->SetText(FText::FromString(text));
	}
}

void UDIalougeWidget::SetDialogueText(const FString& text)
{
	// セリフの文字数をカウント
	int32 LetterCount = text.Len();

	// セリフの文字数分１文字ずつ格納
	m_DialogueLetters.Empty();
	for (int i = 0; i < LetterCount; i++)
	{
		m_DialogueLetters.Add(text.Mid(i, 1));
	}

	// 表示をリセット
	m_CurrentDisplayText = TEXT("");
	m_CurrentLetterIndex = 0;
	m_DialogueText->SetText(FText::FromString(m_CurrentDisplayText));

	// タイマーで1文字ずつ表示
	GetWorld()->GetTimerManager().SetTimer(
		m_LetterTimerHandle,
		this,
		&UDIalougeWidget::ShowNextLetter,
		0.05f,
		true
	);
}

void UDIalougeWidget::ShowNextLetter()
{
	if (!m_DialogueLetters.IsValidIndex(m_CurrentLetterIndex))
	{
		// 全文字表示終了
		GetWorld()->GetTimerManager().ClearTimer(m_LetterTimerHandle);
		return;
	}

	// 文字送りする
	m_CurrentDisplayText += m_DialogueLetters[m_CurrentLetterIndex];
	m_DialogueText->SetText(FText::FromString(m_CurrentDisplayText));
	m_CurrentLetterIndex++;
}

void UDIalougeWidget::SkipTyping()
{
	// タイマーを止める
	GetWorld()->GetTimerManager().ClearTimer(m_LetterTimerHandle);

	// 全文字を即表示
	m_CurrentDisplayText = TEXT("");
	for (const FString& letter : m_DialogueLetters)
	{
		m_CurrentDisplayText += letter;
	}
	m_DialogueText->SetText(FText::FromString(m_CurrentDisplayText));

	m_IsTyping = false;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DIalougeWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UDIalougeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 話者
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_SpeakerText;
	// セリフ
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_DialogueText;

	// 話者取得関数
	void SetSpeakerText(const FString& text);
	// セリフ取得関数
	void SetDialogueText(const FString& text);

	// 1文字表示する関数
	void ShowNextLetter();
	// 文字スキップ関数
	void SkipTyping();

	// 文字送り実行中フラグ取得関数
	bool GetTyping() { return m_IsTyping; }

private:
	// セリフを1文字ずつ格納する配列
	TArray<FString> m_DialogueLetters;
	// 現在表示中の文字列
	FString m_CurrentDisplayText;
	// 現在の文字Index
	int32 m_CurrentLetterIndex;
	// タイマーハンドル
	FTimerHandle m_LetterTimerHandle;

	// 文字送り実行中フラグ
	bool m_IsTyping;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubtitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API USubtitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetSubtitle(const FString& SpeakerName, const FString& Text);

	UFUNCTION(BlueprintCallable)
	void HideSubtitle();

	bool IsTyping() const { return m_IsTyping; }

	UFUNCTION(BlueprintCallable)
	void SkipTyping();

private:
	void TypeNextLetter();

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* m_SpeakerNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* m_SubtitleBodyText;
	
private:
	FString m_FullBodyText;
	FString m_DisplayBodyText;
	int32 m_CurrentLetterIndex;
	FTimerHandle m_TypingTimerHandle;
	bool m_IsTyping;
};

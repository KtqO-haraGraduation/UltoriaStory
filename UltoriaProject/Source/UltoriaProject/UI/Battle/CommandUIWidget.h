// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIBase.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "CommandUIWidget.generated.h"

class UCommandButtonWidget;
class UCommandCursorWidget;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCommandUIWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	// ê∂ê¨éûÇÃèâä˙âªèàóù
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	void UpdateSelection(int index);

private:
	FVector2D GetCursorPos(UCommandButtonWidget* button) const;

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_CommandList;

	UPROPERTY(meta = (BindWidget))
	UCommandCursorWidget* WBP_CommandCursor;
	
	TArray<UCommandButtonWidget*> m_Buttons;
	int m_CurrentIndex;

	bool m_IsInitialized = false;
	int m_PendingIndex = 0;
	int m_InitCount = 0;

	// ä‘ÇÃä‘äu
	static constexpr float CharacterPadding = 1.0f;
};

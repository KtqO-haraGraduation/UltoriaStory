// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleUIBase.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "BattleSelectionUIBase.generated.h"

class UBattleButtonBase;
class UCommandCursorWidget;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleSelectionUIBase : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	void UpdateSelection(int index);

private:
	FVector2D GetCursorPos(UBattleButtonBase* button) const;

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_List;

	UPROPERTY(meta = (BindWidget))
	UCommandCursorWidget* WBP_Cursor;

	TArray<UBattleButtonBase*> m_Buttons;
	
	int m_CurrentIndex;

	bool m_IsInitialized = false;
	int m_PendingIndex = 0;
	int m_InitCount = 0;

	// �Ԃ̊Ԋu
	static constexpr float CharacterPadding = 1.0f;
};

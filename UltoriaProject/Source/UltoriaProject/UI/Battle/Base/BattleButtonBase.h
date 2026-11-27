// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../BattleUIBase.h"
#include "BattleButtonBase.generated.h"

class UBattleSelectionUIBase;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleButtonBase : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetOwner(UBattleSelectionUIBase* owner, int index);
	void SetMyName(FName name);
	virtual void SetSelected(bool flag);

	bool IsSelectable() const { return m_IsSelectable; }
	void SetSelectable(bool flag) { m_IsSelectable = flag; }

protected:
	UFUNCTION()
	virtual void OnClickButton() {} // クリック時の処理

	UFUNCTION()
	virtual void OnHoverButton() {} // 選択中の処理

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Name;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SelectAnim;

	UPROPERTY()
	UBattleSelectionUIBase* m_Owner;

	bool m_IsSelected = false;
	bool m_IsSelectable = true;
	int m_Index = 0;
};

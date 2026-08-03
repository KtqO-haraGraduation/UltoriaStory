// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BattleUIBase.h"
#include "../../Battle/BattleManager.h"
#include "CommandButtonWidget.generated.h"

class UCommandUIWidget;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCommandButtonWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetCommandData(const FBattleCommandData& data);
	void SetOwner(UCommandUIWidget* owner, int index);
	void SetSelected(bool flag);

private:
	UFUNCTION()
	void OnClickCommandButton();

	UFUNCTION()
	void OnHoverCommandButton();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Name;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SelectAnim;

	FBattleCommandData m_CommandData;

	UPROPERTY()
	UCommandUIWidget* m_Owner;

	bool m_IsSelected = false;
	int m_Index = 0;
};

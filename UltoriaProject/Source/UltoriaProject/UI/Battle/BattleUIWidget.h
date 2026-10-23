// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIBase.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "BattleUIWidget.generated.h"

class UCommandUIWidget;
class UBattleSelectionUIBase;
class UUltoriaGaugeWidget;
enum class EBattleUIType;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleUIWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	// ê∂ê¨éûÇÃèâä˙âªèàóù
	virtual void NativeConstruct() override;

	UHorizontalBox* GetHBCharacterStatus() const { return HB_CharacterStatus; }
	UBattleSelectionUIBase* GetCommandWidget() const { return WBP_Command; }
	UUltoriaGaugeWidget* GetUltoriaGauge() const { return UltoriaGaugeWidget; }

	UBattleSelectionUIBase* GetSelectionUI(EBattleUIType type) const;

private:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_CharacterStatus;

	UPROPERTY(meta = (BindWidget))
	UBattleSelectionUIBase* WBP_Command;
	UPROPERTY(meta = (BindWidget))
	UBattleSelectionUIBase* WBP_Skill;
	UPROPERTY(meta = (BindWidget))
	UBattleSelectionUIBase* WBP_Item;

	UPROPERTY(meta = (BindWidget))
	UUltoriaGaugeWidget* UltoriaGaugeWidget;

	UPROPERTY(meta = (BindWiget))
	UTextBlock* DescriptionText;
};

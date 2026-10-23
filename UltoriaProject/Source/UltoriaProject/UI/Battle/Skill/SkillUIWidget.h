// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BattleSelectionUIBase.h"
#include "Components/TextBlock.h"
#include "SkillUIWidget.generated.h"

class USkillBaseComponent;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API USkillUIWidget : public UBattleSelectionUIBase
{
	GENERATED_BODY()
	
public:
	void CreateButton() override;
	void SetButtonData() override;

	void UpdateSelection(int index) override;

private:
	USkillBaseComponent* GetCurrentSkillComponent();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescText;
};

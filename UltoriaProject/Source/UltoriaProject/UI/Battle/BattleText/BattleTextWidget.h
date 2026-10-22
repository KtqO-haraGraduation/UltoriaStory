// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleUIBase.h"
#include "Components/TextBlock.h"
#include "BattleTextWidget.generated.h"

// テキストのテンプレートの種類
enum class ETextTemplate : uint8
{
	SKILL_USE,
	ITEM_USE,
	HEAL,
	BUFF,
	DAMAGE,
	STATUS_CHANGE,
	DEAD,
	MISS,
	ESCAPE,
	GUARD,
	CRITICAL,
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleTextWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	void SetItemText(FText text);
	void SetSkillText(FText text);

	void ResetText();

private:
	UPROPERTY(meta = (BlnWidget))
	UTextBlock* BattleText;
};

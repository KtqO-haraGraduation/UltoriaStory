// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Base/BattleButtonBase.h"
#include "../../../Battle/BattleManager.h"
#include "SkillButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API USkillButtonWidget : public UBattleButtonBase
{
	GENERATED_BODY()
	
public:
	void SetSkillData(FName skillName);

protected:
	void OnClickButton() override;
	void OnHoverButton() override;

};

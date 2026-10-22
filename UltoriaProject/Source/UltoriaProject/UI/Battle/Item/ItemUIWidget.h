// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BattleSelectionUIBase.h"
#include "ItemUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UItemUIWidget : public UBattleSelectionUIBase
{
	GENERATED_BODY()
	
public:
	void CreateButton() override;
	void SetButtonData() override;
};

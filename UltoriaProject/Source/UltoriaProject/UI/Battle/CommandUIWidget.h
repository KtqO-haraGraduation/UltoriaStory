// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BattleSelectionUIBase.h"
#include "CommandUIWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCommandUIWidget : public UBattleSelectionUIBase
{
	GENERATED_BODY()
	
public:
	void CreateButton() override;
	void UpdateSelection(int index) override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescText;
};

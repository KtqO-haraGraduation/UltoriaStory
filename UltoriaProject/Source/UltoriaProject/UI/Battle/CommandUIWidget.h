// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BattleSelectionUIBase.h"
#include "CommandUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCommandUIWidget : public UBattleSelectionUIBase
{
	GENERATED_BODY()
	
public:
	// ¶¬‚Ì‰Šú‰»ˆ—
	virtual void NativeConstruct() override;

	void CreateButton() override;
};

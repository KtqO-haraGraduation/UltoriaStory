// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Base/BattleButtonBase.h"
#include "ItemButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UItemButtonWidget : public UBattleButtonBase
{
	GENERATED_BODY()
	
protected:
	void OnClickButton() override;
	void OnHoverButton() override;
};

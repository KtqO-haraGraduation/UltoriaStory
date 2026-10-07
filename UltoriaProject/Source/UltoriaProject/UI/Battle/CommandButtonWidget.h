// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BattleButtonBase.h"
#include "../../Battle/BattleManager.h"
#include "CommandButtonWidget.generated.h"

class UCommandUIWidget;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCommandButtonWidget : public UBattleButtonBase
{
	GENERATED_BODY()
	
public:
	void SetCommandData(const FBattleCommandData& data);
	void SetSelected(bool flag) override;

protected:
	void OnClickButton() override;
	void OnHoverButton() override;

private:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SelectAnim;

	FBattleCommandData m_CommandData;
};

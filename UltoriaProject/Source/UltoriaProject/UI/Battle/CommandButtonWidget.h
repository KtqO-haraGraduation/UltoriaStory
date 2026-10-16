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

protected:
	void OnClickButton() override;
	void OnHoverButton() override;

private:
	FBattleCommandData m_CommandData;
};

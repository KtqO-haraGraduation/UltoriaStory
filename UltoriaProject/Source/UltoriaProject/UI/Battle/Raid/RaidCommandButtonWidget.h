// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BattleButtonBase.h"
#include "../../../Battle/Raid/RaidBattleTypes.h"
#include "RaidCommandButtonWidget.generated.h"

UCLASS()
class ULTORIAPROJECT_API URaidCommandButtonWidget : public UBattleButtonBase
{
	GENERATED_BODY()

public:
	void SetRaidCommand(ERaidCommandType command, const FText& label);

protected:
	virtual void OnClickButton()override;
	virtual void OnHoverButton()override;

private:
	ERaidCommandType m_Command = ERaidCommandType::ATTACK;
};

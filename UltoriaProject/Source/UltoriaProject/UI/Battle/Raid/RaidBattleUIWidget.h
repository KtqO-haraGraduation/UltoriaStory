// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleUIBase.h"
#include "RaidBattleUIWidget.generated.h"

class ARaidBattleManager;
class ACharacterBase;
class UHorizontalBox;
class URaidCommandUIWidget;
class URaidSpeedUIWidget;

UCLASS()
class ULTORIAPROJECT_API URaidBattleUIWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	void SetRaidManager(ARaidBattleManager* manager);

	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

	UFUNCTION()
	void RefreshRaidUI();

private:
	void SetTurnHightlight(ACharacterBase* character, bool bOn);

	UPROPERTY(Transient)
	TObjectPtr<ARaidBattleManager> m_RaidManager;

	TWeakObjectPtr<ACharacterBase> m_LastCurrent;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_CharacterStatus;

	UPROPERTY(meta = (BindWidget))
	URaidCommandUIWidget* WBP_Command;

	UPROPERTY(meta = (BindWidget))
	URaidSpeedUIWidget* WBP_Speed;
};

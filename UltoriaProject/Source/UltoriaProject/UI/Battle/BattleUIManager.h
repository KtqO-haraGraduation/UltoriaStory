// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageWidget.h"
#include "Subsystems/WorldSubsystem.h"
#include "BattleUIManager.generated.h"

class ACharacterBase;
class UBattleUIWidget;
class UBattleResultWidget;
class UBattleUIWidget;
class UBattleResultWidget;

enum class EBattleUIType;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleUIManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void CreateBattleUI();
	void CreateResultUI();
	void RemoveBattleUI();
	void RemoveResultUI();

	void UpdateSelection(EBattleUIType type, int index);
	void SetUIVisible(EBattleUIType type, bool visible);

	void CreateDamageWidget(ACharacterBase* target, int damage);

	// UIÇÃçXêV
	void OnTurnChanged(ACharacterBase* prevCharacter, ACharacterBase* currentCharacter);

	UBattleUIWidget* GetBattleUI() const { return m_BattleUIWidget; }
	UBattleResultWidget* GetResultUI() const { return m_ResultUIWidget; }

private:
	void SetTargetSelectionUI(bool flag);
	void SetAllTargetSelectionUI(bool flag);

private:
	UPROPERTY()
	UBattleUIWidget* m_BattleUIWidget;
	UBattleResultWidget* m_ResultUIWidget;
};

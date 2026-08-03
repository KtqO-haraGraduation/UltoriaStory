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
class UDamageWidget;

enum class EBattleUIType
{
	NONE,

	STATUS,
	COMMAND,
	TARGET,
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleUIManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void CreateBattleUI();
	void CreateResultUI();
	void RemoveBattleUI();
	void RemoveResultUI();

	void UpdateCommandSelection(int index);
	void SetUIVisible(EBattleUIType type, bool visible);

	void CreateDamageWidget(ACharacterBase* target, int damage);

	// UIの更新
	void OnTurnChanged(ACharacterBase* prevCharacter, ACharacterBase* currentCharacter);

private:
	// バトルに必要なデータをロードする
	void LoadBattle();

	void SetEnemySelectionUI(bool flag);

private:
	UPROPERTY()
	UBattleUIWidget* m_BattleUIWidget;
	UBattleResultWidget* m_ResultUIWidget;

	TSubclassOf<UBattleUIWidget> m_BattleWidgetClass;
	TSubclassOf<UBattleResultWidget> m_ResultWidgetClass;
	TSubclassOf<UDamageWidget> m_DamageWidgetClass;
};

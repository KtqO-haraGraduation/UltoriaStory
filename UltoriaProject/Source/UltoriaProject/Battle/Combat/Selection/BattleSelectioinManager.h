// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Data/BattleCommandData.h"
#include "../../../System/Selection/SelectionSystem.h"
#include "../../../Job/Skill/SkillDataRow.h"
#include "BattleSelectioinManager.generated.h"

class UBattleManager;
class ACharacterBase;
class AEnemyBase;
enum class EUIInputType;

// 選択肢の種類
UENUM(BlueprintType)
enum class SelectionType : uint8
{
	NONE,
	COMMAND,
	TARGET,
	SKILL,
	ITEM,
	RESULT,
};

using FSelectionMoveFunc = void (UBattleSelectioinManager::*)(EUIInputType);
using FSelectionConfrmFunc = void (UBattleSelectioinManager::*)();
using FSelectionCancelFunc = void (UBattleSelectioinManager::*)();

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleSelectioinManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(UBattleManager* battleMg);

	void SelectMoveTarget(EUIInputType type);	// 選択カーソルの移動
	void SelectConfirm();						// 選択の決定
	void SelectCancel();						// 選択のキャンセル

	SelectionType GetSelectionType() const { return m_SelectionType; }
	AEnemyBase* GetEnemyCurrentSelect();
	TSelectionSystem<ACharacterBase*>& GetEnemySelection() { return m_EnemySelection; }
	TSelectionSystem<FBattleCommandData>& GetCommandSelection() { return m_CommandSelection; }

	void SetSelectionType(SelectionType type) { m_SelectionType = type; }
	void SetEnemyTarget(const TArray<ACharacterBase*>& targets);
	void SetSkillTarget();
	void SetCommandSelectIndex(int index);
	void SetSkillSelectIndex(int index);

	void OnEnemySelectionChanged(int oldIndex, int newIndex);
	void OnCommandSelectionChanged(int oldIndex, int newIndex);
	void OnSkillSelectionChanged(int oldIndex, int newIndex);

private:
	void MoveCommand(EUIInputType type);
	void MoveTarget(EUIInputType type);
	void MoveSkill(EUIInputType type);
	void MoveItem(EUIInputType type);
	void MoveResult(EUIInputType type);

	void ConfirmCommand();
	void ConfirmTarget();
	void ConfirmSkill();
	void ConfirmItem();
	void ConfirmResult();

	void CancelCommand();
	void CancelTarget();
	void CancelSkill();
	void CancelItem();
	void CancelResult();

private:
	UBattleManager* m_BattleManager;
	SelectionType m_SelectionType;				// 現在の選択処理の種類

	TSelectionSystem<ACharacterBase*> m_EnemySelection;			// 敵の選択システム
	TSelectionSystem<FBattleCommandData> m_CommandSelection;	// コマンドの選択システム
	TSelectionSystem<FSkillDataRow> m_SkillSelection;			// スキルの選択システム

	// 選択タイプごとの処理
	TMap<SelectionType, FSelectionMoveFunc> m_MoveFunc;
	TMap<SelectionType, FSelectionConfrmFunc> m_ConfrmFunc;
	TMap<SelectionType, FSelectionCancelFunc> m_CancelFunc;
};

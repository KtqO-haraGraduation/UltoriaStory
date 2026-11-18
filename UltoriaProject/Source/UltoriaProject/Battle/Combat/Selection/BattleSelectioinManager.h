// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Data/BattleCommandData.h"
#include "../../../System/Selection/SelectionSystem.h"
#include "../../../Job/Skill/SkillDataRow.h"
#include "../../../Inventory/InventorySubsystem.h"
#include "BattleSelectioinManager.generated.h"

class UBattleManager;
class ACharacterBase;
class AEnemyBase;
enum class EUIInputType;
enum class EBattleUIType;

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

// キャラクターごとのインデックス
struct FCharacterSelectionIndex
{
	int commandIndex = 0;
	int skillIndex = 0;
	int itemIndex = 0;
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
	SelectionType GetPrevSelectionType() const { return m_PrevSelectionType; }
	ACharacterBase* GetTargetCurrentSelect();

	TSelectionSystem<ACharacterBase*>& GetTargetSelection() { return m_TargetSelection; }
	TSelectionSystem<FBattleCommandData>& GetCommandSelection() { return m_CommandSelection; }

	void SetSelectionType(SelectionType type) { m_SelectionType = type; }
	
	void SetCommandTarget();
	void SetCharacterTarget(const TArray<ACharacterBase*>& targets);
	void SetSkillTarget();
	void SetItemTarget();

	void SetCommandSelectIndex(int index);
	void SetSkillSelectIndex(int index);
	void SetItemSelectIndex(int index);

	void OnTargetSelectionChanged(int oldIndex, int newIndex);
	void OnCommandSelectionChanged(int oldIndex, int newIndex);
	void OnSkillSelectionChanged(int oldIndex, int newIndex);
	void OnItemSelectionChanged(int oldIndex, int newIndex);

	TArray<ACharacterBase*> BeginTargetSelection(ETargetTeam team);

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

	void SetSelectionChanged();
	void ResetSelection();

private:
	bool m_IsCanMoveTargetCursor;				// ターゲットカーソルが動かせるか

	UBattleManager* m_BattleManager;
	SelectionType m_PrevSelectionType;			// 前回の選択処理の種類
	SelectionType m_SelectionType;				// 現在の選択処理の種類

	TSelectionSystem<ACharacterBase*> m_TargetSelection;		// ターゲットの選択システム
	TSelectionSystem<FBattleCommandData> m_CommandSelection;	// コマンドの選択システム
	TSelectionSystem<FSkillDataRow> m_SkillSelection;			// スキルの選択システム
	TSelectionSystem<FInventoryItem> m_ItemSelection;			// アイテムの選択システム

	// 選択タイプごとの処理
	TMap<SelectionType, FSelectionMoveFunc> m_MoveFunc;
	TMap<SelectionType, FSelectionConfrmFunc> m_ConfrmFunc;
	TMap<SelectionType, FSelectionCancelFunc> m_CancelFunc;

	FCharacterSelectionIndex m_CharacterIndex[4];
};

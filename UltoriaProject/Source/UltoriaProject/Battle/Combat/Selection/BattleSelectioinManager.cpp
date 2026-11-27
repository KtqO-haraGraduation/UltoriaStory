// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSelectioinManager.h"
#include "../../../UI/Battle/BattleUIManager.h"
#include "../../../System/Controller/PlayPlayerController.h"
#include "../../../Character/CharacterBase.h"
#include "../../../Character/Enemy/EnemyBase.h"
#include "../../../Job/Skill/SkillBaseComponent.h"
#include "../../../Akiduki/ItemEffect/ItemUseSubsystem.h"
#include "../../../Akiduki/ItemData/ItemData.h"
#include "../../../Data/Item/ItemDataManager.h"
#include "../../BattleManager.h"
#include "../Components/CombatComponent.h"
#include "../Data/BattleDataManager.h"

void UBattleSelectioinManager::Init(UBattleManager* battleMg)
{
	if (!battleMg) return;
	m_BattleManager = battleMg;
	m_SelectionType = SelectionType::NONE;
	m_PrevSelectionType = SelectionType::NONE;
	m_IsCanMoveTargetCursor = true;

	// 登録する前に前回のイベントを削除
	ResetSelection();

	// カーソル移動時の処理
	SetSelectionChanged();

	// コマンドの選択データを入れる
	SetCommandTarget();
	// アイテムの選択データを入れる
	SetItemTarget();

	m_MoveFunc.Add(SelectionType::COMMAND, &UBattleSelectioinManager::MoveCommand);
	m_MoveFunc.Add(SelectionType::TARGET, &UBattleSelectioinManager::MoveTarget);
	m_MoveFunc.Add(SelectionType::SKILL, &UBattleSelectioinManager::MoveSkill);
	m_MoveFunc.Add(SelectionType::ITEM, &UBattleSelectioinManager::MoveItem);
	m_MoveFunc.Add(SelectionType::RESULT, &UBattleSelectioinManager::MoveResult);

	m_ConfrmFunc.Add(SelectionType::COMMAND, &UBattleSelectioinManager::ConfirmCommand);
	m_ConfrmFunc.Add(SelectionType::TARGET, &UBattleSelectioinManager::ConfirmTarget);
	m_ConfrmFunc.Add(SelectionType::SKILL, &UBattleSelectioinManager::ConfirmSkill);
	m_ConfrmFunc.Add(SelectionType::ITEM, &UBattleSelectioinManager::ConfirmItem);
	m_ConfrmFunc.Add(SelectionType::RESULT, &UBattleSelectioinManager::ConfirmResult);

	m_CancelFunc.Add(SelectionType::COMMAND, &UBattleSelectioinManager::CancelCommand);
	m_CancelFunc.Add(SelectionType::TARGET, &UBattleSelectioinManager::CancelTarget);
	m_CancelFunc.Add(SelectionType::SKILL, &UBattleSelectioinManager::CancelSkill);
	m_CancelFunc.Add(SelectionType::ITEM, &UBattleSelectioinManager::CancelItem);
	m_CancelFunc.Add(SelectionType::RESULT, &UBattleSelectioinManager::CancelResult);
}

void UBattleSelectioinManager::SelectMoveTarget(EUIInputType type)
{
	if (FSelectionMoveFunc* func = m_MoveFunc.Find(m_SelectionType))
	{
		(this->* * func)(type);
	}
}

void UBattleSelectioinManager::SelectConfirm()
{
	if (FSelectionConfrmFunc* func = m_ConfrmFunc.Find(m_SelectionType))
	{
		(this->* * func)();
	}
}

void UBattleSelectioinManager::SelectCancel()
{
	if (FSelectionCancelFunc* func = m_CancelFunc.Find(m_SelectionType))
	{
		(this->* * func)();
	}
}

ACharacterBase* UBattleSelectioinManager::GetTargetCurrentSelect()
{
	return m_TargetSelection.GetCurrent();
}

void UBattleSelectioinManager::SetCommandTarget()
{
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (battleDataMg) m_CommandSelection.SetTargets(battleDataMg->GetCommandDatas());
}

void UBattleSelectioinManager::SetCharacterTarget(const TArray<ACharacterBase*>& targets)
{
	m_TargetSelection.SetTargets(targets);
}

void UBattleSelectioinManager::SetSkillTarget()
{
	ACharacterBase* currentCharacter = m_BattleManager->GetCurrentCharacter();
	if (!currentCharacter) return;

	USkillBaseComponent* skillComponent = currentCharacter->GetComponent<USkillBaseComponent>();
	if (!skillComponent) return;

	m_SkillSelection.SetTargets(skillComponent->GetSkills());
}

void UBattleSelectioinManager::SetItemTarget()
{
	UInventorySubsystem* inventory = GetInventory();
	if (!inventory) return;

	m_ItemSelection.SetTargets(inventory->GetInventory());
}

void UBattleSelectioinManager::SetCommandSelectIndex(int index)
{
	m_CommandSelection.SetCurrentIndex(index);
}

void UBattleSelectioinManager::SetSkillSelectIndex(int index)
{
	m_SkillSelection.SetCurrentIndex(index);
}

void UBattleSelectioinManager::SetItemSelectIndex(int index)
{
	m_ItemSelection.SetCurrentIndex(index);
}

void UBattleSelectioinManager::OnTargetSelectionChanged(int oldIndex, int newIndex)
{
	const auto& targets = m_TargetSelection.GetTargets();

	if (!targets.IsValidIndex(oldIndex) ||
		!targets.IsValidIndex(newIndex))
	{
		return;
	}

	ACharacterBase* oldCharacter = targets[oldIndex];
	ACharacterBase* newCharacter = targets[newIndex];

	if (oldCharacter)
	{
		oldCharacter->SetSelectionUIVisibility(false);
	}
	if (newCharacter)
	{
		newCharacter->SetSelectionUIVisibility(true);
	}
}

void UBattleSelectioinManager::OnCommandSelectionChanged(int oldIndex, int newIndex)
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->UpdateSelection(EBattleUIType::COMMAND, newIndex);
}

void UBattleSelectioinManager::OnSkillSelectionChanged(int oldIndex, int newIndex)
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->UpdateSelection(EBattleUIType::SKILL, newIndex);
}

void UBattleSelectioinManager::OnItemSelectionChanged(int oldIndex, int newIndex)
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->UpdateSelection(EBattleUIType::ITEM, newIndex);
}

TArray<ACharacterBase*> UBattleSelectioinManager::BeginTargetSelection(ETargetTeam team)
{
	return m_BattleManager->GetActiveTargets(team);
}

// --------- Move関数 --------- //
void UBattleSelectioinManager::MoveCommand(EUIInputType type)
{
	switch (type)
	{
	case EUIInputType::UP:
		m_CommandSelection.Move(-1);
		break;
	case EUIInputType::DOWN:
		m_CommandSelection.Move(1);
		break;
	}
}

void UBattleSelectioinManager::MoveTarget(EUIInputType type)
{
	if (!m_IsCanMoveTargetCursor) return;

	switch (type)
	{
	case EUIInputType::LEFT:
		m_TargetSelection.Move(-1);
		break;
	case EUIInputType::RIGHT:
		m_TargetSelection.Move(1);
		break;
	}
}

void UBattleSelectioinManager::MoveSkill(EUIInputType type)
{
	switch (type)
	{
	case EUIInputType::UP:
		m_SkillSelection.Move(-1);
		break;
	case EUIInputType::DOWN:
		m_SkillSelection.Move(1);
		break;
	}
}

void UBattleSelectioinManager::MoveItem(EUIInputType type)
{
	switch (type)
	{
	case EUIInputType::UP:
		m_ItemSelection.Move(-1);
		break;
	case EUIInputType::DOWN:
		m_ItemSelection.Move(1);
		break;
	}
}

void UBattleSelectioinManager::MoveResult(EUIInputType type)
{

}

// --------- Confirm関数 --------- //
void UBattleSelectioinManager::ConfirmCommand()
{
	const FBattleCommandData& command = m_CommandSelection.GetCurrent();
	m_BattleManager->ExecuteCommand(command.Type);

	m_PrevSelectionType = SelectionType::COMMAND;
}

void UBattleSelectioinManager::ConfirmTarget()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	ACharacterBase* currentCharacter = m_BattleManager->GetCurrentCharacter();
	if (!currentCharacter) return;

	UCombatComponent* combat = currentCharacter->GetComponent<UCombatComponent>();
	if (!combat) return;

	// 前回の選択肢によって分岐
	if (m_PrevSelectionType == SelectionType::COMMAND)
	{
		m_BattleManager->ExecuteAttack(m_TargetSelection.GetCurrent());
	}
	else if (m_PrevSelectionType == SelectionType::SKILL)
	{
		// スキル使用
		USkillBaseComponent* skillComponent = m_BattleManager->GetCurrentCharacter()->GetComponent<USkillBaseComponent>();
		if (!skillComponent) return;

		const FSkillDataRow* skillData = m_SkillSelection.GetCurrent();
		if (!skillData) return;

		if (skillData->TargetType == ETargetType::SINGLE)
		{
			TArray<ACharacterBase*> targets = {};
			targets.Add(m_TargetSelection.GetCurrent());

			skillComponent->UseSkill(m_SkillSelection.GetCurrent(), targets);
		}
		else if (skillData->TargetType == ETargetType::ALL)
		{
			skillComponent->UseSkill(m_SkillSelection.GetCurrent(), m_TargetSelection.GetTargets());
		}
	}
	else if (m_PrevSelectionType == SelectionType::ITEM)
	{
		UItemDataManager* itemDataMg =
			GetWorld()->GetSubsystem<UItemDataManager>();

		if (!itemDataMg) return;

		UItemUseSubsystem* itemUse = GetItemUseSubsystem();
		if (!itemUse) return;

		const FItemData* itemData =
			itemDataMg->GetItemData(m_ItemSelection.GetCurrent().ItemID);

		if (!itemData) return;

		if (itemData->Target == EItemUseTarget::AllAllies ||
			itemData->Target == EItemUseTarget::AllEnemies)
		{
			itemUse->UseItemBattle(*itemData, m_TargetSelection.GetTargets());
		}
		else
		{
			TArray<ACharacterBase*> targets;
			targets.Add(m_TargetSelection.GetCurrent());

			itemUse->UseItemBattle(*itemData, targets);
		}

		combat->NextTurn();
	}

	uiMg->SetUIVisible(EBattleUIType::TARGET, false);
	uiMg->SetUIVisible(EBattleUIType::ALLTARGET, false);
	m_IsCanMoveTargetCursor = true;
}

void UBattleSelectioinManager::ConfirmSkill()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	USkillBaseComponent* skillComponent = m_BattleManager->GetCurrentCharacter()->GetComponent<USkillBaseComponent>();
	if (!skillComponent) return;

	const FSkillDataRow* skillData = m_SkillSelection.GetCurrent();
	if (!skillData) return;

	TArray<ACharacterBase*> targets = BeginTargetSelection(skillData->TargetTeam);

	ShowTargetSelection(
		targets, 
		skillData->TargetTeam, 
		skillData->TargetType);

	m_PrevSelectionType = SelectionType::SKILL;

	uiMg->SetUIVisible(EBattleUIType::SKILL, false);
	uiMg->SetUIVisible(EBattleUIType::TARGET, true);
}

void UBattleSelectioinManager::ConfirmItem()
{
	// インベントリが空だったら何もしない
	UInventorySubsystem* inventory = GetInventory();
	if (!inventory || inventory->GetInventory().IsEmpty()) return;

	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	UItemDataManager* itemDataMg = GetWorld()->GetSubsystem<UItemDataManager>();
	if (!itemDataMg) return;

	// 現在選択しているアイテムのデータ
	const FItemData* itemData = itemDataMg->GetItemData(m_ItemSelection.GetCurrent().ItemID);

	// アイテムのターゲットからTargetTeamに変換
	ETargetTeam targetTeam = ToSkillTargetTeam(itemData->Target);
	// アイテムのターゲットからTargetTypeに変換
	ETargetType targetType = ToSkillTargetType(itemData->Target);

	TArray<ACharacterBase*> targets = BeginTargetSelection(targetTeam);

	ShowTargetSelection(
		targets, 
		targetTeam, 
		targetType);

	m_PrevSelectionType = SelectionType::ITEM;

	uiMg->SetUIVisible(EBattleUIType::ITEM, false);
}

void UBattleSelectioinManager::ConfirmResult()
{
	// リザルトの現在の進行度によって処理を分ける

	// Lv上昇演出

	// ジョブLv上昇演出

	// スキル解放演出

	// 全ての演出が終わって、ボタンを押したらバトル終了

	m_BattleManager->FinishBattle();
}

// --------- Cancel関数 --------- //
void UBattleSelectioinManager::CancelCommand()
{

}

void UBattleSelectioinManager::CancelTarget()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	// コマンド選択に戻る
	m_SelectionType = SelectionType::COMMAND;
	m_PrevSelectionType = SelectionType::NONE;
	m_IsCanMoveTargetCursor = true;

	uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
	uiMg->SetUIVisible(EBattleUIType::TARGET, false);
}

void UBattleSelectioinManager::CancelSkill()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	m_SelectionType = SelectionType::COMMAND;
	m_PrevSelectionType = SelectionType::NONE;
	m_IsCanMoveTargetCursor = true;

	uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
	uiMg->SetUIVisible(EBattleUIType::TARGET, false);
	uiMg->SetUIVisible(EBattleUIType::SKILL, false);
}

void UBattleSelectioinManager::CancelItem()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	m_SelectionType = SelectionType::COMMAND;
	m_PrevSelectionType = SelectionType::NONE;
	m_IsCanMoveTargetCursor = true;

	uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
	uiMg->SetUIVisible(EBattleUIType::ITEM, false);
}

void UBattleSelectioinManager::CancelResult()
{

}

void UBattleSelectioinManager::SetSelectionChanged()
{
	// 敵のカーソル移動時のイベント
	m_TargetSelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleSelectioinManager::OnTargetSelectionChanged
	);

	// コマンドのカーソル移動時のイベント
	m_CommandSelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleSelectioinManager::OnCommandSelectionChanged
	);

	// スキルのカーソル移動時のイベント
	m_SkillSelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleSelectioinManager::OnSkillSelectionChanged
	);

	// アイテムのカーソル移動時のイベント
	m_ItemSelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleSelectioinManager::OnItemSelectionChanged
	);
}

void UBattleSelectioinManager::ResetSelection()
{
	m_TargetSelection.m_OnSelectionChanged.Unbind();
	m_CommandSelection.m_OnSelectionChanged.Unbind();
	m_SkillSelection.m_OnSelectionChanged.Unbind();
	m_ItemSelection.m_OnSelectionChanged.Unbind();
}

void UBattleSelectioinManager::ShowTargetSelection(const TArray<ACharacterBase*>& targets, ETargetTeam team, ETargetType type)
{
	if (type == ETargetType::ALL || team == ETargetTeam::SELF)
	{
		m_IsCanMoveTargetCursor = false;
	}
	m_BattleManager->ShowTargetUI(targets, type);
}

UInventorySubsystem* UBattleSelectioinManager::GetInventory()
{
	// インベントリーシステムを返す
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (!gameInstance) return nullptr;

	UInventorySubsystem* inventory = gameInstance->GetSubsystem<UInventorySubsystem>();
	if (!inventory) return nullptr;

	return inventory;
}

UItemUseSubsystem* UBattleSelectioinManager::GetItemUseSubsystem()
{
	// インベントリーシステムを返す
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (!gameInstance) return nullptr;

	UItemUseSubsystem* itemUse = gameInstance->GetSubsystem<UItemUseSubsystem>();
	if (!itemUse) return nullptr;

	return itemUse;
}

ETargetTeam UBattleSelectioinManager::ToSkillTargetTeam(EItemUseTarget type)
{
	switch (type)
	{
	case EItemUseTarget::Ally:
	case EItemUseTarget::AllAllies:
		return ETargetTeam::ALLY;

	case EItemUseTarget::Enemy:
	case EItemUseTarget::AllEnemies:
		return ETargetTeam::ENEMY;

	case EItemUseTarget::Self:
		return ETargetTeam::SELF;

	default:
		return ETargetTeam::SELF;
	}
}

ETargetType UBattleSelectioinManager::ToSkillTargetType(EItemUseTarget type)
{
	switch (type)
	{
	case EItemUseTarget::AllAllies:
	case EItemUseTarget::AllEnemies:
		return ETargetType::ALL;

	case EItemUseTarget::Ally:
	case EItemUseTarget::Enemy:
	case EItemUseTarget::Self:
		return ETargetType::SINGLE;

	default:
		return ETargetType::SINGLE;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSelectioinManager.h"
#include "../../../UI/Battle/BattleUIManager.h"
#include "../../../System/Controller/PlayPlayerController.h"
#include "../../../Character/CharacterBase.h"
#include "../../../Character/Enemy/EnemyBase.h"
#include "../../../Job/Skill/SkillBaseComponent.h"
#include "../../BattleManager.h"
#include "../Data/BattleDataManager.h"

void UBattleSelectioinManager::Init(UBattleManager* battleMg)
{
	if (!battleMg) return;
	m_BattleManager = battleMg;
	m_SelectionType = SelectionType::NONE;

	// 登録する前に前回のイベントを削除
	ResetSelection();

	// カーソル移動時の処理
	SetSelectionChanged();

	// コマンドの選択データを入れる
	SetCommandTarget();

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


	// インベントリにアイテム（仮）を追加
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (!gameInstance) return;

	UInventorySubsystem* inventory = gameInstance->GetSubsystem<UInventorySubsystem>();
	if (!inventory) return;

	inventory->AddItem("Item1", 2);
	inventory->AddItem("Item2", 1);
	inventory->AddItem("Item3", 5);

	SetItemTarget();
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

AEnemyBase* UBattleSelectioinManager::GetTargetCurrentSelect()
{
	return Cast<AEnemyBase>(m_TargetSelection.GetCurrent());
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
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (!gameInstance) return;

	UInventorySubsystem* inventory = gameInstance->GetSubsystem<UInventorySubsystem>();
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

	AEnemyBase* oldEnemy = Cast<AEnemyBase>(targets[oldIndex]);
	AEnemyBase* newEnemy = Cast<AEnemyBase>(targets[newIndex]);

	if (oldEnemy)
	{
		oldEnemy->SetSelectionUIVisibility(false);
	}
	if (newEnemy)
	{
		newEnemy->SetSelectionUIVisibility(true);
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
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return TArray<ACharacterBase*>();

	return battleMg->GetActiveTargets(team);
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
}

void UBattleSelectioinManager::ConfirmTarget()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	// 一旦、選択した敵に攻撃処理
	m_BattleManager->ExecuteAttack(m_TargetSelection.GetCurrent());
	uiMg->SetUIVisible(EBattleUIType::TARGET, false);
}

void UBattleSelectioinManager::ConfirmSkill()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	USkillBaseComponent* skillComponent = battleMg->GetCurrentCharacter()->GetComponent<USkillBaseComponent>();
	if (!skillComponent) return;

	FSkillDataRow skillData = m_SkillSelection.GetCurrent();
	TArray<ACharacterBase*> targets = BeginTargetSelection(skillData.TargetTeam);

	skillComponent->UseSkill(m_SkillSelection.GetCurrentIndex(), targets);

	//if (skillData.TargetType == ETargetType::SINGLE)
	//{
	//	// 選択画面へ
	//	battleMg->ShowTargetUI(targets);
	//}
	//else if(skillData.TargetType == ETargetType::ALL)
	//{
	//	battleMg->ShowTargetUI(targets);

	//	skillComponent->UseSkill(m_SkillSelection.GetCurrentIndex(), targets);
	//}

	uiMg->SetUIVisible(EBattleUIType::SKILL, false);
	uiMg->SetUIVisible(EBattleUIType::TARGET, true);
}

void UBattleSelectioinManager::ConfirmItem()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->SetUIVisible(EBattleUIType::ITEM, false);
}

void UBattleSelectioinManager::ConfirmResult()
{
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

	uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
	uiMg->SetUIVisible(EBattleUIType::TARGET, false);
}

void UBattleSelectioinManager::CancelSkill()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	m_SelectionType = SelectionType::COMMAND;

	uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
	uiMg->SetUIVisible(EBattleUIType::SKILL, false);
}

void UBattleSelectioinManager::CancelItem()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	m_SelectionType = SelectionType::COMMAND;

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
